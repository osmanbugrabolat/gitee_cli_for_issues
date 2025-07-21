#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "token_manager.hpp"
#include "issue_manager.hpp"

using namespace std;

#include <iostream>
using namespace std;

void print_help() {
    const string bold = "\033[1m";
    const string reset = "\033[0m";
    const string cyan = "\033[36m";
    const string yellow = "\033[33m";

    cout << bold << "Gitee CLI Tool" << reset << "\n\n";

    cout << bold << "Usage:" << reset << "\n";
    cout << "  " << cyan << "gitee-cli help" << reset << "\n"
         << "      Show this help message.\n\n";

    cout << "  " << cyan << "gitee-cli store --owner <owner> --token <token>" << reset << "\n"
         << "      Store your personal access token securely.\n\n";

    cout << "  " << cyan << "gitee-cli create-issue --owner <owner> --repo <repo> --title <title> --body <body> --issue_type <type>" << reset << "\n"
         << "      Create a new issue in the specified repository.\n\n";

    cout << "  " << cyan << "gitee-cli update-issue --owner <owner> --repo <repo> --number <n> [--title <title>] [--body <body>] [--state <state>]" << reset << "\n"
         << "      Update an existing issue.\n\n";

    cout << "  " << cyan << "gitee-cli list-issues --owner <owner> --repo <repo>" << reset << "\n"
         << "      List all issues in the specified repository.\n\n";

    cout << bold << "Options:" << reset << "\n";
    cout << "  " << yellow << "--owner, -o" << reset << "       Repository owner\n";
    cout << "  " << yellow << "--repo, -r" << reset << "        Repository name\n";
    cout << "  " << yellow << "--token, -k" << reset << "       Personal access token\n";
    cout << "  " << yellow << "--title, -t" << reset << "       Issue title\n";
    cout << "  " << yellow << "--body, -b" << reset << "        Issue body\n";
    cout << "  " << yellow << "--issue_type, -i" << reset << "  Issue type\n";
    cout << "  " << yellow << "--number, -n" << reset << "      Issue number\n";
    cout << "  " << yellow << "--state, -s" << reset << "       Issue state (open/closed)\n";
}


unordered_map<string, string> parse_args(int argc, char* argv[], int start_idx) {
    unordered_map<string, string> args_map;

    for (int i = start_idx; i < argc; ++i) {
        string arg = argv[i];
        if ((arg.rfind("--", 0) == 0 || arg.rfind("-", 0) == 0) && (i + 1) < argc) {
            string value = argv[i + 1];
            // Normalize keys, örneğin --owner veya -o ikilisini "owner" olarak tutabiliriz
            if (arg == "--owner" || arg == "-o") args_map["owner"] = value;
            else if (arg == "--repo" || arg == "-r") args_map["repo"] = value;
            else if (arg == "--token" || arg == "-t") args_map["token"] = value;
            else if (arg == "--title" || arg == "-T") args_map["title"] = value;
            else if (arg == "--body" || arg == "-b") args_map["body"] = value;
            else if (arg == "--issue_type" || arg == "-i") args_map["issue_type"] = value;
            else if (arg == "--number" || arg == "-n") args_map["number"] = value;
            else if (arg == "--state" || arg == "-s") args_map["state"] = value;
            else {
                cerr << "Unknown argument: " << arg << endl;
                exit(1);
            }
            ++i; // atla value kısmını da işledik
        } else {
            cerr << "Invalid or missing value for argument: " << arg << endl;
            exit(1);
        }
    }

    return args_map;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    string command = argv[1];
    if (command == "help") {
        print_help();
        return 0;
    }

    unordered_map<string, string> args = parse_args(argc, argv, 2);

    if (command == "store") {
        if (args.find("owner") == args.end() || args.find("token") == args.end()) {
            cerr << "Error: --owner and --token are required for store command.\n";
            return 1;
        }
        if (store_token_to_keyring(args["owner"], args["token"]))
            cout << "Token saved successfully.\n";
        else
            cerr << "Failed to save token.\n";
        return 0;
    }
    else if (command == "create-issue") {
        if (args.find("owner") == args.end() || args.find("repo") == args.end() ||
            args.find("title") == args.end() || args.find("body") == args.end() ||
            args.find("issue_type") == args.end()) {
            cerr << "Error: --owner, --repo, --title, --body, and --issue_type are required for create-issue.\n";
            return 1;
        }
        return create_issue(args["owner"], args["repo"], args["title"], args["body"], args["issue_type"]);
    }
    else if (command == "update-issue") {
        if (args.find("owner") == args.end() || args.find("repo") == args.end() || args.find("number") == args.end()) {
            cerr << "Error: --owner, --repo and --number are required for update-issue.\n";
            return 1;
        }
        string title = args.count("title") ? args["title"] : "";
        string body  = args.count("body") ? args["body"] : "";
        string state = args.count("state") ? args["state"] : "";
        return update_issue(args["owner"], args["repo"], args["number"], title, body, state);
    }
    else if (command == "list-issues") {
        if (args.find("owner") == args.end() || args.find("repo") == args.end()) {
            cerr << "Error: --owner and --repo are required for list-issues.\n";
            return 1;
        }
        return list_issues(args["owner"], args["repo"]);
    }

    cerr << "Unknown command: " << command << endl;
    return 1;
}


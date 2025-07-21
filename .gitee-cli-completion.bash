# ~/.gitee-cli-completion.bash

_gitee_cli_complete() {
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    if [[ $prev == "create-issue" || $prev == "update-issue" ]]; then
        if [[ -f ~/.config/gitee-cli/repos.txt ]]; then
            mapfile -t repos < ~/.config/gitee-cli/repos.txt
            COMPREPLY=( $(compgen -W "${repos[*]}" -- "$cur") )
        fi
        return 0
    fi
}

complete -F _gitee_cli_complete ./gitee-cli

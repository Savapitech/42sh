/*
** EPITECH PROJECT, 2025
** __
** File description:
** _
*/

#ifndef GIT_H
    #define GIT_H
    #define MAX_LINE 256
    #define MAX_BRANCH 128
    #define HASH_LEN 41

typedef struct {
    char branch[MAX_BRANCH];
    char local_hash[HASH_LEN];
    char remote_hash[HASH_LEN];
    int ahead;
    int behind;
} git_status_t;

bool get_git_status(git_status_t *status);
#endif /* GIT_H */

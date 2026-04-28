#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    char username[50];
    char password[50];
};

struct Post {
    char username[50];
    char content[200];
    int likes;
};

// 🔐 Register
void registerUser() {
    FILE *fp = fopen("users.txt", "a");
    struct User u;

    printf("Enter username: ");
    scanf("%s", u.username);

    printf("Enter password: ");
    scanf("%s", u.password);

    fwrite(&u, sizeof(u), 1, fp);
    fclose(fp);

    printf("Registration successful!\n");
}

// 🔑 Login
int loginUser(char currentUser[]) {
    FILE *fp = fopen("users.txt", "r");
    struct User u;
    char username[50], password[50];

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    while (fread(&u, sizeof(u), 1, fp)) {
        if (strcmp(u.username, username) == 0 &&
            strcmp(u.password, password) == 0) {
            strcpy(currentUser, username);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// 📝 Create Post
void createPost(char currentUser[]) {
    FILE *fp = fopen("posts.txt", "a");
    struct Post p;

    strcpy(p.username, currentUser);
    p.likes = 0;

    getchar(); // clear buffer

    printf("Enter your post: ");
    fgets(p.content, sizeof(p.content), stdin);

    fwrite(&p, sizeof(p), 1, fp);
    fclose(fp);

    printf("Post added!\n");
}

// 📜 View Posts
void viewPosts() {
    FILE *fp = fopen("posts.txt", "r");
    struct Post p;
    int i = 1;

    printf("\n--- Posts ---\n");

    while (fread(&p, sizeof(p), 1, fp)) {
        printf("\n%d. %s", i++, p.content);
        printf("By: %s\nLikes: %d\n", p.username, p.likes);
    }

    fclose(fp);
}

// ❤️ Like Post
void likePost() {
    FILE *fp = fopen("posts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    struct Post p;
    int choice, i = 1;

    printf("\n--- Select Post to Like ---\n");

    while (fread(&p, sizeof(p), 1, fp)) {
        printf("%d. %s (Likes: %d)\n", i++, p.content, p.likes);
    }

    printf("Enter post number: ");
    scanf("%d", &choice);

    rewind(fp);
    i = 1;

    while (fread(&p, sizeof(p), 1, fp)) {
        if (i == choice) {
            p.likes++;
        }
        fwrite(&p, sizeof(p), 1, temp);
        i++;
    }

    fclose(fp);
    fclose(temp);

    remove("posts.txt");
    rename("temp.txt", "posts.txt");

    printf("Liked successfully!\n");
}

// 🏠 Main Menu
int main() {
    int choice;
    char currentUser[50];

    while (1) {
        printf("\n1. Register\n2. Login\n3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            registerUser();
        }
        else if (choice == 2) {
            if (loginUser(currentUser)) {
                printf("Login successful! Welcome %s\n", currentUser);

                int ch;
                while (1) {
                    printf("\n1. Create Post\n2. View Posts\n3. Like Post\n4. Logout\n");
                    printf("Enter choice: ");
                    scanf("%d", &ch);

                    if (ch == 1) createPost(currentUser);
                    else if (ch == 2) viewPosts();
                    else if (ch == 3) likePost();
                    else break;
                }
            } else {
                printf("Invalid login!\n");
            }
        }
        else {
            break;
        }
    }

    return 0;
}
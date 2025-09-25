#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 512

int CujoCloudConnectivity(char const* str)
{
    char buff[BUFFER_SIZE];
    memset(buff, 0, sizeof(buff));
    char value[32] = "200";
    FILE * fp = popen( str, "r" );
    if ( fp == NULL )
    {
        return -1;
    }

    fgets(buff, sizeof(buff), fp);
    pclose(fp);

    if (strncmp(buff,value,strlen(value)) == 0)
    {
       return 0;
    }
    else
    {
       return 1;
    }
}

int CujoAgentProcessStatus(char const* str)
{
    char buff[BUFFER_SIZE];
    memset(buff, 0, sizeof(buff));
    char value[32] = "1";
    FILE * fp = popen( str, "r" );
    if ( fp == NULL )
    {
        return -1;
    }

    fgets(buff, sizeof(buff), fp);
    pclose(fp);

    if (strncmp(buff,value,strlen(value)) == 0)
    {
       return 0;
    }
    else
    {
       return 1;
    }
}

int CcspAdvsecProcessStatus(char const* str)
{
    char buff[BUFFER_SIZE];
    memset(buff, 0, sizeof(buff));
    char value[32] = "1";
    FILE * fp = popen( str, "r" );
    if ( fp == NULL )
    {
        return -1;
    }

    fgets(buff, sizeof(buff), fp);
    pclose(fp);

    if (strncmp(buff,value,strlen(value)) == 0)
    {
       return 0;
    }
    else
    {
       return 1;
    }
}

int CcspAdvsecFeatureTr181(char const* str)
{
    char buff[BUFFER_SIZE];
    memset(buff, 0, sizeof(buff));
    char value[32] = "true";
    FILE * fp = popen( str, "r" );
    if ( fp == NULL )
    {
        return -1;
    }

    fgets(buff, sizeof(buff), fp);
    pclose(fp);

    if (strncmp(buff,value,strlen(value)) == 0)
    {
       return 0;
    }
    else
    {
       return 1;
    }
}

int CcspAdvSecAgentStatus(const char* commands[], int numCommands) {
    const char* expectedValue = "true";
    char buff[BUFFER_SIZE] = {0};

    for (int i = 0; i < numCommands; i++) {
        FILE* fp = popen(commands[i], "r");
        if (fp == NULL) {
            return -1; // Return an error code
        }

        if (fgets(buff, sizeof(buff), fp) == NULL) {
            pclose(fp);
            return -1; // Return an error code
        }

        pclose(fp);

        if (strncmp(buff, expectedValue, strlen(expectedValue)) != 0) {
            return 1; // Return 1 if the output does not match the expected value
        }

        memset(buff, 0, sizeof(buff)); // Clear the buffer for the next command
    }

    return 0; // Return 0 if all commands produce the expected value
}

int executeAndCheck(const char *command, const char *expected) {
    FILE *fp;
    char buffer[512] = {0};

    // Execute the Linux command and capture the output
    fp = popen(command, "r");
    if (fp == NULL) {
        return -1;
    }

    // Read the output and check for the expected string
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, expected) != NULL) {
            pclose(fp);
            return 0; // Expected string found, return 0
        }
    }

    pclose(fp);
    return 1; // Expected string not found, return 1
}

int CcspAdvSecAgent_SetToTrue(const char* commands[]) {

    const char* expected[] = {"succeed", "0", "true", "true"};
    if (executeAndCheck(commands[0], expected[0]) != 0) {
        return 1;
    }

    sleep(45);
    if (executeAndCheck(commands[1], expected[1]) != 0) {
        return 2;
    }

    if (executeAndCheck(commands[2], expected[2]) != 0) {
        return 3;
    }

    if (executeAndCheck(commands[3], expected[3]) != 0) {
        return 4;
    }

    return 0;
}

int AdvSecSafeBrowsing_SetToTrue(const char* commands[]) {

    const char* expected[] = {"succeed", "0", "true", "true", "SAFEBRO"};
    if (executeAndCheck(commands[0], expected[0]) != 0) {
        return 1;
    }

    sleep(45);
    if (executeAndCheck(commands[1], expected[1]) != 0) {
        return 2;
    }

    if (executeAndCheck(commands[2], expected[2]) != 0) {
        return 3;
    }

    if (executeAndCheck(commands[3], expected[3]) != 0) {
        return 4;
    }

    if (executeAndCheck(commands[4], expected[4]) != 0) {
        return 5;
    }

    return 0;
}

int AdvSecCujoTelemetryWiFiFP_SetToTrue(const char* commands[]) {

    const char* expected[] = {"succeed", "0", "true", "true"};
    if (executeAndCheck(commands[0], expected[0]) != 0) {
        return 1;
    }

    sleep(45);
    if (executeAndCheck(commands[1], expected[1]) != 0) {
        return 2;
    }

    if (executeAndCheck(commands[2], expected[2]) != 0) {
        return 3;
    }

    if (executeAndCheck(commands[3], expected[3]) != 0) {
        return 4;
    }

    return 0;
}

int AdvSecCujoTelemetryWiFiFP_SetToFalse(const char* commands[]) {

    const char* expected[] = {"succeed", "0", "false", "false"};
    if (executeAndCheck(commands[0], expected[0]) != 0) {
        return 1;
    }

    sleep(45);
    if (executeAndCheck(commands[1], expected[1]) != 0) {
        return 2;
    }

    if (executeAndCheck(commands[2], expected[2]) != 0) {
        return 3;
    }

    if (executeAndCheck(commands[3], expected[3]) != 0) {
        return 4;
    }

    return 0;
}

int AdvSecSafeBrowsing_SetToFalse(const char* commands[]) {

    const char* expected[] = {"succeed", "0", "false", "false", "0"};
    if (executeAndCheck(commands[0], expected[0]) != 0) {
        return 1;
    }

    sleep(45);
    if (executeAndCheck(commands[1], expected[1]) != 0) {
        return 2;
    }

    if (executeAndCheck(commands[2], expected[2]) != 0) {
        return 3;
    }

    if (executeAndCheck(commands[3], expected[3]) != 0) {
        return 4;
    }

    if (executeAndCheck(commands[4], expected[4]) != 0) {
        return 5;
    }

    return 0;
}

int CcspAdvSecAgent_SetToFalse(const char* commands[]) {

    const char* expected[] = {"succeed", "2", "false"};
    if (executeAndCheck(commands[0], expected[0]) != 0) {
        return 1;
    }

    sleep(45);
    if (executeAndCheck(commands[1], expected[1]) != 0) {
        return 2;
    }

    if (executeAndCheck(commands[2], expected[2]) != 0) {
        return 3;
    }

    return 0;
}
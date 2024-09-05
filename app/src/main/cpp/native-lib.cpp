#include <jni.h>
#include <string>
#include <cstdlib>
//#include "libnode/include/node/node.h"
#include "node.h"

#include <pthread.h>
#include <unistd.h>
#include <android/log.h>



int pipe_stdout[2];
int pipe_stderr[2];
pthread_t thread_stout;
pthread_t thread_stderr;
const char *ADBTAG = "NODEJS";

void *thread_stderr_func(void*){
    ssize_t redirect_size;
    char buf[2048];
    while ((redirect_size = read(pipe_stderr[0], buf, sizeof buf - 1)) > 0){
        if(buf[redirect_size - 1] == '\n')
            --redirect_size;
        buf[redirect_size] = 0;
        __android_log_write(ANDROID_LOG_ERROR, ADBTAG, buf);
    }
    return 0;
}

void *thread_stdout_func(void*){
    ssize_t redirect_size;
    char buf[2048];
    while ((redirect_size = read(pipe_stdout[0], buf, sizeof buf -1)) > 0 ){
        if(buf[redirect_size - 1] == '\n')
            --redirect_size;
        buf[redirect_size] = 0;
        __android_log_write(ANDROID_LOG_INFO, ADBTAG, buf);
    }
    return 0;
}

int start_redirecting_stdout_stderr(){
    setvbuf(stdout, 0, _IONBF, 0);
    pipe(pipe_stdout);
    dup2(pipe_stdout[1],STDOUT_FILENO);

    setvbuf(stderr, 0, _IONBF, 0);
    pipe(pipe_stderr);
    dup2(pipe_stderr[1], STDERR_FILENO);

    if(pthread_create(&thread_stout, 0, thread_stdout_func, 0) == -1)
        return  -1;
    pthread_detach(thread_stout);
    if (pthread_create(&thread_stderr, 0, thread_stderr_func, 0) == -1)
        return  -1;
    pthread_detach(thread_stderr);

    return 0;
}



extern "C" jint JNICALL
//Java_com_yourorg_sample_MainActivity_startNodeWithArguments(
Java_com_example_myapplication_MainActivity_startNodeWithArguments(
        JNIEnv *env,
        jobject /* this */,
        jobjectArray arguments) {

    jsize argumentCount = env->GetArrayLength(arguments);

    int argumentsSize = 0;
    for (int i = 0; i < argumentCount ; i++) {
        argumentsSize += strlen(env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arguments, i), 0));
        argumentsSize++; // for '\0'
    }

    char* args_buffer = (char*) calloc(argumentsSize, sizeof(char));

    char* arg[argumentCount];

    char* currentArgsPosition = args_buffer;

    for (int i = 0; i < argumentCount ; i++)
    {
        const char* currentArgument = env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arguments, i), 0);

        strncpy(currentArgsPosition, currentArgument, strlen(currentArgument));

        arg[i] = currentArgsPosition;

        currentArgsPosition += strlen(currentArgsPosition) + 1;
    }


//    int node_result = node::Start(argumentCount, arg);
//    free(args_buffer);

//    return jint(node_result);

    if (start_redirecting_stdout_stderr() == -1){
        __android_log_write(ANDROID_LOG_ERROR, ADBTAG, "not start stdout/stderr to logcat");
    }

    free(args_buffer);
    return jint (node::Start(argumentCount,arg));

}


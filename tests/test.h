#ifndef TEST_H
#define TEST_H

    #ifndef TFNAME
    #define TFNAME "TEST"  // <filename> used for testing
    #endif

struct test_options {
    int t_csens;
    int t_pmatch;
    int t_home;
    int t_openf;
    int t_perm;
    int t_sys;
} t_option;

#endif
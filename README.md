# cppTest2
a better version of cpp test to demo the "automatical" way of initialize search path and builtin macro 

In previous cppTest, tons of builtin-macro are hardcoded. Looking at cc1[plus] which can generate by translating user input options. Similarly is search path.

Based on that, we need a tons of static libs, those are similar to cppTest. However, we need more "gcc-building" statlic lib like isl, mpz, mpfr, gmp, decnumber, z and dl which is dynamic lib. They can be found in gcc-build directory. However, to simplify linking command, I create all symlink to my workspace directory.

/home/nick/Downloads/gcc-10.2.0/gcc-10.2.0-debug-libcpp-install/bin/g++ -L"/home/nick/eclipse-2021/parserTest" -o "parserTest"  ./src/parserTest.o   -lcfamily -lparser -lcfamily -lcpp -lgcc_main -lcommon-target -lcommon_special -liberty -lbacktrace -lisl -lmpc -lmpz -lmpfr -lgmp -ldecnumber -lz -ldl


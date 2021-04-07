# cppTest2
a better version of cpp test to demo the "automatical" way of initialize search path and builtin macro 

In previous cppTest, tons of builtin-macro are hardcoded. Looking at cc1[plus] which can generate by translating user input options. Similarly is search path.

Based on that, we need a tons of static libs, those are similar to cppTest. However, we need more "gcc-building" statlic lib like isl, mpz, mpfr, gmp, decnumber, z and dl which is dynamic lib. They can be found in gcc-build directory. However, to simplify linking command, I create all symlink to my workspace directory.

/home/nick/Downloads/gcc-10.2.0/gcc-10.2.0-debug-libcpp-install/bin/g++ -L"/home/nick/eclipse-2021/parserTest" -o "parserTest"  ./src/parserTest.o   -lcfamily -lparser -lcfamily -lcpp -lgcc_main -lcommon-target -lcommon_special -liberty -lbacktrace -lisl -lmpc -lmpz -lmpfr -lgmp -ldecnumber -lz -ldl

cfamily: 
it is create by including all gcc/c-family/*.o.  i.e. ar rvs libcfamily $(ls *.o)

parser:
similarly, it is created by all obj in gcc/cp. ar rvs libparser.a $(ls *.o)

gcc_main: this is based on original libbackend.a which is a thin archive. So, first convert this thin archive by 
for i in $(ar t libbackend.a); do ar rvs libbackend_real.a "$i"; done
And then adding following obj to it to make a new libgcc_main.a. see it diffs like this:

ar t libbackend_real.a | sort > /tmp/back.txt
ar t libgcc_main.a | sort > /tmp/gcc.txt
comm --check-order -3 /tmp/back.txt /tmp/gcc.txt
	attribs.o
	cc1plus-checksum.o
	glibc-c.o
	i386-c.o
  
common_special: it is based on original libcommon.a by removing ggc-none.o
in other words, it is equivalent to: libcommon.a  <====>   ar d libcommon.a gcc-none.o
This is because we need ggc-page.o which is conflict with ggc-none.o  

Finally, static linking order is VERY IMPORTANT. ORDER MATTERS!

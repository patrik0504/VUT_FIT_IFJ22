<?php
declare(strict_types=1);

// write(" ");

// write("sym1: 2; sym2: 0\n");
// if(2>0) {write("true1");} else {}
// if(2>=0) {write("true2");} else {}
// if(2<0) {write("true3");} else {}
// if(2<0) {write("true4");} else {}
// write("\n\n");

// write("sym1: 0; sym2: 2\n");
// if(0>2) {write("true1");} else {}
// if(0>=2) {write("true2");} else {}
// if(0<2) {write("true3");} else {}
// if(0<=2) {write("true4");} else {}
// write("\n\n");

// write("sym1: 2.0; sym2: 0\n"); // VALID!! V IFJCode nutná konverze na float
// if(2.0>0) {write("true1");} else {}
// if(2.0>=0) {write("true2");} else {}
// if(2.0<0) {write("true3");} else {}
// if(2.0<=0) {write("true4");} else {}
// write("\n\n");

// write("sym1: 0.0; sym2: 2.0\n");
// if(0.0>2.0) {write("true1");} else {}
// if(0.0>=2.0) {write("true2");} else {}
// if(0.0<2.0) {write("true3");} else {}
// if(0.0<=2.0) {write("true4");} else {}
// write("\n\n");

// write("sym1: aaaa; sym2: aa\n");
// if("aaaa">"aa") {write("true1");} else {}
// if("aaaa">="aa") {write("true2");} else {}
// if("aaaa"<"aa") {write("true3");} else {}
// if("aaaa"<="aa") {write("true4");} else {}
// write("\n\n");

// write("sym1: aaaa; sym2: aaaa\n");
// if("aaaa">"aaaa") {write("true1");} else {}
// if("aaaa">="aaaa") {write("true2");} else {}
// if("aaaa"<"aaaa") {write("true3");} else {}
// if("aaaa"<="aaaa") {write("true4");} else {}
// write("\n\n");

if (0-1 >= 0-1) {} else { write("fail 0-1 >= 0-1\n"); }
if (0-1 >= 0) { write("fail 0-1 >= 0\n"); } else {}
if (0-1 >= 1) { write("fail 0-1 >= 1\n"); } else {}
if (0-1 >= 0-1.0) {} else { write("fail 0-1 >= 0-1.0\n"); }
if (0-1 >= 0-0.5) { write("fail 0-1 >= 0-0.5\n"); } else {}
if (0-1 >= 0.0) { write("fail 0-1 >= 0.0\n"); } else {}
if (0-1 >= 0.5) { write("fail 0-1 >= 0.5\n"); } else {}
if (0-1 >= 1.0) { write("fail 0-1 >= 1.0\n"); } else {}
if (0 >= 0-1) {} else { write("fail 0 >= 0-1\n"); }
if (0 >= 0) {} else { write("fail 0 >= 0\n"); }
if (0 >= 1) { write("fail 0 >= 1\n"); } else {}
if (0 >= 0-1.0) {} else { write("fail 0 >= 0-1.0\n"); }
if (0 >= 0-0.5) {} else { write("fail 0 >= 0-0.5\n"); }
if (0 >= 0.0) {} else { write("fail 0 >= 0.0\n"); }
if (0 >= 0.5) { write("fail 0 >= 0.5\n"); } else {}
if (0 >= 1.0) { write("fail 0 >= 1.0\n"); } else {}
if (1 >= 0-1) {} else { write("fail 1 >= 0-1\n"); }
if (1 >= 0) {} else { write("fail 1 >= 0\n"); }
if (1 >= 1) {} else { write("fail 1 >= 1\n"); }
if (1 >= 0-1.0) {} else { write("fail 1 >= 0-1.0\n"); }
if (1 >= 0-0.5) {} else { write("fail 1 >= 0-0.5\n"); }
if (1 >= 0.0) {} else { write("fail 1 >= 0.0\n"); }
if (1 >= 0.5) {} else { write("fail 1 >= 0.5\n"); }
if (1 >= 1.0) {} else { write("fail 1 >= 1.0\n"); }
if (0-1.0 >= 0-1) {} else { write("fail 0-1.0 >= 0-1\n"); }
if (0-1.0 >= 0) { write("fail 0-1.0 >= 0\n"); } else {}
if (0-1.0 >= 1) { write("fail 0-1.0 >= 1\n"); } else {}
if (0-1.0 >= 0-1.0) {} else { write("fail 0-1.0 >= 0-1.0\n"); }
if (0-1.0 >= 0-0.5) { write("fail 0-1.0 >= 0-0.5\n"); } else {}
if (0-1.0 >= 0.0) { write("fail 0-1.0 >= 0.0\n"); } else {}
if (0-1.0 >= 0.5) { write("fail 0-1.0 >= 0.5\n"); } else {}
if (0-1.0 >= 1.0) { write("fail 0-1.0 >= 1.0\n"); } else {}
if (0-0.5 >= 0-1) {} else { write("fail 0-0.5 >= 0-1\n"); }
if (0-0.5 >= 0) { write("fail 0-0.5 >= 0\n"); } else {}
if (0-0.5 >= 1) { write("fail 0-0.5 >= 1\n"); } else {}
if (0-0.5 >= 0-1.0) {} else { write("fail 0-0.5 >= 0-1.0\n"); }
if (0-0.5 >= 0-0.5) {} else { write("fail 0-0.5 >= 0-0.5\n"); }
if (0-0.5 >= 0.0) { write("fail 0-0.5 >= 0.0\n"); } else {}
if (0-0.5 >= 0.5) { write("fail 0-0.5 >= 0.5\n"); } else {}
if (0-0.5 >= 1.0) { write("fail 0-0.5 >= 1.0\n"); } else {}
if (0.0 >= 0-1) {} else { write("fail 0.0 >= 0-1\n"); }
if (0.0 >= 0) {} else { write("fail 0.0 >= 0\n"); }
if (0.0 >= 1) { write("fail 0.0 >= 1\n"); } else {}
if (0.0 >= 0-1.0) {} else { write("fail 0.0 >= 0-1.0\n"); }
if (0.0 >= 0-0.5) {} else { write("fail 0.0 >= 0-0.5\n"); }
if (0.0 >= 0.0) {} else { write("fail 0.0 >= 0.0\n"); }
if (0.0 >= 0.5) { write("fail 0.0 >= 0.5\n"); } else {}
if (0.0 >= 1.0) { write("fail 0.0 >= 1.0\n"); } else {}
if (0.5 >= 0-1) {} else { write("fail 0.5 >= 0-1\n"); }
if (0.5 >= 0) {} else { write("fail 0.5 >= 0\n"); }
if (0.5 >= 1) { write("fail 0.5 >= 1\n"); } else {}
if (0.5 >= 0-1.0) {} else { write("fail 0.5 >= 0-1.0\n"); }
if (0.5 >= 0-0.5) {} else { write("fail 0.5 >= 0-0.5\n"); }
if (0.5 >= 0.0) {} else { write("fail 0.5 >= 0.0\n"); }
if (0.5 >= 0.5) {} else { write("fail 0.5 >= 0.5\n"); }
if (0.5 >= 1.0) { write("fail 0.5 >= 1.0\n"); } else {}
if (1.0 >= 0-1) {} else { write("fail 1.0 >= 0-1\n"); }
if (1.0 >= 0) {} else { write("fail 1.0 >= 0\n"); }
if (1.0 >= 1) {} else { write("fail 1.0 >= 1\n"); }
if (1.0 >= 0-1.0) {} else { write("fail 1.0 >= 0-1.0\n"); }
if (1.0 >= 0-0.5) {} else { write("fail 1.0 >= 0-0.5\n"); }
if (1.0 >= 0.0) {} else { write("fail 1.0 >= 0.0\n"); }
if (1.0 >= 0.5) {} else { write("fail 1.0 >= 0.5\n"); }
if (1.0 >= 1.0) {} else { write("fail 1.0 >= 1.0\n"); }
if ("" >= "") {} else { write("fail \"\" >= \"\"\n"); }
if ("" >= "aa") { write("fail \"\" >= \"aa\"\n"); } else {}
if ("" >= "ab") { write("fail \"\" >= \"ab\"\n"); } else {}
if ("aa" >= "") {} else { write("fail \"aa\" >= \"\"\n"); }
if ("aa" >= "aa") {} else { write("fail \"aa\" >= \"aa\"\n"); }
if ("aa" >= "ab") { write("fail \"aa\" >= \"ab\"\n"); } else {}
if ("ab" >= "") {} else { write("fail \"ab\" >= \"\"\n"); }
if ("ab" >= "aa") {} else { write("fail \"ab\" >= \"aa\"\n"); }
if ("ab" >= "ab") {} else { write("fail \"ab\" >= \"ab\"\n"); }
if (0.0 >= 0.0) {} else { write("fail 0.0 >= 0.0\n"); }
if (0.0 >= 0) {} else { write("fail 0.0 >= 0\n"); }
if (0.0 >= null) {} else { write("fail 0.0 >= null\n"); }
if (0 >= 0.0) {} else { write("fail 0 >= 0.0\n"); }
if (0 >= 0) {} else { write("fail 0 >= 0\n"); }
if (0 >= null) {} else { write("fail 0 >= null\n"); }
if ("" >= "") {} else { write("fail \"\" >= \"\"\n"); }
if (" " >= null) {} else { write("fail \"\" >= null\n"); }
if (null >= 0.0) {} else { write("fail null >= 0.0\n"); }
if (null >= 0) {} else { write("fail null >= 0\n"); }
if (null >= " ") {} else { write("fail null >= \"\"\n"); }
if (null >= null) {} else { write("fail null >= null\n"); }
write("done");


// write("sym1: \$a (int = 2); sym2: \$b (float = 0.0)\n");
// $a = 2;
// $b = 0.0;
// if($a>$b) {write("true1");} else {}
// if($a>=$b) {write("true2");} else {}
// if($a<$b) {write("true3");} else {}
// if($a<=$b) {write("true3");} else {}
// write("\n\n");

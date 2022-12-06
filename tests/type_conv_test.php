<?php
declare(strict_types=1);

write("sym1: 2; sym2: 0\n");
if(2>0) {write("true1");} else {}
if(2>=0) {write("true2");} else {}
if(2<0) {write("true3");} else {}
if(2<0) {write("true4");} else {}
// if(2>=0) write("true2");
// if(2<0) write("true3");
// if(2<=0) write("true4");
write("\n\n");

write("sym1: 0; sym2: 2\n");
if(0>2) {write("true1");} else {}
if(0>=2) {write("true2");} else {}
if(0<2) {write("true3");} else {}
if(0<=2) {write("true4");} else {}
// if(0>=2) write("true2");
// if(0<2) write("true3");
// if(0<=2) write("true4");
write("\n\n");

write("sym1: 2.0; sym2: 0\n"); // VALID!! V IFJCode nutná konverze na float
if(2.0>0) {write("true1");} else {}
if(2.0>=0) {write("true2");} else {}
if(2.0<0) {write("true3");} else {}
if(2.0<=0) {write("true4");} else {}
// if(2.0>=0) write("true2");
// if(2.0<0) write("true3");
// if(2.0<=0) write("true4");
write("\n\n");

write("sym1: 0.0; sym2: 2.0\n");
if(0.0>2.0) {write("true1");} else {}
if(0.0>=2.0) {write("true2");} else {}
if(0.0<2.0) {write("true3");} else {}
if(0.0<=2.0) {write("true4");} else {}
// if(0.0>=2.0) write("true2");
// if(0.0<2.0) write("true3");
// if(0.0<=2.0) write("true4");
write("\n\n");

write("sym1: aaaa; sym2: aa\n");
if("aaaa">"aa") {write("true1");} else {}
if("aaaa">="aa") {write("true2");} else {}
if("aaaa"<"aa") {write("true3");} else {}
if("aaaa"<="aa") {write("true4");} else {}
// if("aaaa">="aa") write("true2");
// if("aaaa"<"aa") write("true3");
// if("aaaa"<="aa") write("true4");
write("\n\n");

write("sym1: aaaa; sym2: aaaa\n");
if("aaaa">"aaaa") {write("true1");} else {}
if("aaaa">="aaaa") {write("true2");} else {}
if("aaaa"<"aaaa") {write("true3");} else {}
if("aaaa"<="aaaa") {write("true4");} else {}
// if("aaaa">="aaaa") write("true2");
// if("aaaa"<"aaaa") write("true3");
// if("aaaa"<="aaaa") write("true4");
write("\n\n");

write("null check\n");
// if(null<=0) write("null1");
// if(null<0) write("null2");
// if(null>=0) write("null3");
if(null <= 0) { write("null1"); } else { }
if(null < 0) { write("null2"); } else { }
if(null >= 0) { write("null3"); } else { }
if(null > 0) { write("null4"); } else { }
write("\n\n");


write("sym1: \$a (int = 2); sym2: \$b (float = 0.0)\n");
$a = 2;
$b = 0.0;
if($a>$b) {write("true1");} else {}
if($a>=$b) {write("true2");} else {}
if($a<$b) {write("true3");} else {}
if($a<=$b) {write("true3");} else {}
write("\n\n");

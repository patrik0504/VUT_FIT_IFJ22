<?php 
declare(strict_types=1);

// $a = 1+2;
// $bbb = $a + 20 - 7;
// $ccc = $bbb + 6;
// write($ccc);






// $b = 3;
// $c = 2;
function foo(?int $a) : void {
    $bbb = ($a + 2 * (3 + 7))/3;
    $error = 6 + 2;
    $concat = "IFJ "."swag";
    write($bbb, "\n", $concat, "\n");
    return;
}
foo(1);



//$a = 2 + 3;

//$bbb = $a+$b+$c;

/*function some(int $a, int $b, int $c) : int {
    
}
$a = 5;
$b = 6;
$c = 7;

$d = $a - $b * $c;
if($a === 3){
    $bbb = 5;
}
else{
    $ccc = 6;
}
while($a === 3){
    $abcd = 5;
}
some($bbb, $ccc, $abcd);

function test1() : int {
    $a = 3;
    $b = 4;
    if($a === 3){
        $a = 33;
        $a = $a - 1;
        $c = $a;
        some($a, $b, $c);
    }
    else{
        $a = 33;
        $a = $a - 1;
        $c = $a;
        some($a, $b, $c);
    }
}

function test2(int $a, int $b, float $ads, string $tojesuper) : int {
    $acc = 5;
    some($a, $b, $acc);
    while($a === 3){
        $a = 33;
        $a = $a - 1;
        $c = $a;
        some($a, $b, $c);
    }
    return $a+$b;
}
*/
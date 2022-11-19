<?php 
declare(strict_types=1);

function some(int $a, int $b, int $c) : int {
    
}

some($a, $b, $c);


while($a === 3){
    $a = 33;
    $a = $a - 1;
    some($a, $b, $c);
}
if($a === 3){
    if($a === 3){
        $a = 33;
        $a = $a - 1;
        some($a, $b, $c);
    }
    else{
        $a = 33;
        $a = $a - 1;
        some($a, $b, $c);
    }
    $a = 33;
    $a = $a - 1;
    some($a, $b, $c);
}
else{
    $a = 33;
    $a = $a - 1;
    some($a, $b, $c);
}

function test1() : int {
    $a = 3;
    if($a === 3){
        $a = 33;
        $a = $a - 1;
        some($a, $b, $c);
    }
    else{
        $a = 33;
        $a = $a - 1;
        some($a, $b, $c);
    }
}

function test2(int $a, int $b, float $ads, string $tojesuper) : int {
    while($a === 3){
        $a = 33;
        $a = $a - 1;
        some($a, $b, $c);
    }
}
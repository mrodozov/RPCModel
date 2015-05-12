<?php 

$result = file_get_contents($argv[1]);
$decoded_array = json_decode($result,true);
// dump it
$array_of_keys = array_keys($decoded_array);
foreach($array_of_keys as $a_value) {
    $array_of_ls=$decoded_array[$a_value];
    $sum_of_good_lumi = 0;
    foreach($array_of_ls as $one_record) {
        $sum_of_good_lumi += ($one_record[1] - $one_record[0]) + 1 ;
    }
    /*
    if($sum_of_good_lumi < 100){
    echo $a_value ; echo " "; echo $sum_of_good_lumi;
    echo "\n";
    }
    */
}

foreach($array_of_keys as $val) {
    echo $val; echo "\n";
}



/*
foreach($decoded_array as $val) {
//     var_dump($val);
  $one_run_records = sizeof($val);
  echo sizeof($val);echo "\n";
}
*/

?>
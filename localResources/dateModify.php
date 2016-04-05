<?php
$zone = new DateTimeZone('UTC');
$thefile = file($argv[1]);
$dateFile ; $timeFile;

foreach($thefile as $line) {

  $linearray = explode(" ",$line);
  $linearray[1] = str_replace(".root","",$linearray[1]);

  $datetime_file = explode("__",$linearray[1]);


  $datetime_file[0] = str_replace("_","-",$datetime_file[0]);
  $datetime_file[1] = str_replace("_",":",$datetime_file[1]);

  $date1 = new DateTime($datetime_file[0].' '.$datetime_file[1],$zone);
  

  $date2 = new DateTime($linearray[2].' '.$linearray[3], $zone);
  $correction = substr($linearray[4],0,1)." hours";

  date_sub($date2,DateInterval::createFromDateString($correction)); // corrects the date and time of the $date2 object with the second parameter (one or two hours)                                                                            
  date_sub($date1,DateInterval::createFromDateString($correction));
  
  
  echo $linearray[0] . " ";
  echo $date1->format('m/d/y H:i:s') . " ";
  echo $date2->format('m/d/y H:i:s') . "\n";

}


?>
<?php

$m1 = new Memcache;
$m1->addServer("localhost", 11211);

$m2 = new Memcache;
$m2->addServer("localhost", 11211);

$timeout = 25;
$sleept = 16;

$m1->delete("k1");
$m1->set("k1", "whats her name");

echo "\nKey k1 is set";
echo "\nDoing getl of k1 with 20 second timeout \n";

var_dump($m1->getl("k1", $timeout));
echo "\nSleeping for $sleept seconds\n";
sleep($sleept);

echo "\nSleep done. Doing getl of k1 again\n";
var_dump($m2->getl("k1"));
echo "\nLets try a set instead\n";
var_dump($m2->set("k1", "lucy"));
echo "\nLets try a set instead\n";
var_dump($m2->set("k1", "lucy"));
echo "\nA getl once more:\n";
var_dump($m2->getl("k1"));
echo "\nDone\n";

?>


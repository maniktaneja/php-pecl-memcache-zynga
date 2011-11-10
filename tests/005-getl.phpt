--TEST--
check setMultiByKey method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$m1 = new Memcache;
$m1->addServer("192.168.1.4", 11211);

$m2 = new Memcache;
$m2->addServer("192.168.1.4", 11211);

$timeout = 3;
$sleept = 1;

$m1->delete("k1");
$m1->set("k1", "whats her name");

echo "\nKey k1 is set";
echo "\nDoing getl of k1 with 3 second timeout \n";

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
--EXPECT--
Key k1 is set
Doing getl of k1 with 3 second timeout 
string(14) "whats her name"

Sleeping for 1 seconds

Sleep done. Doing getl of k1 again
bool(false)

Lets try a set instead
bool(false)

Lets try a set instead
bool(false)

A getl once more:
bool(false)

Done
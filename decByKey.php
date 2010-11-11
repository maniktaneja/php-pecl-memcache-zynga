<?php

require_once("init.php");

$memcache = new memcache;

$memcache->debug(DEBUG);
$memcache->addServer("localhost", 11211);
$memcache->addServer("localhost1", 11211);
$memcache->addServer("localhost2", 11211);
$memcache->addServer("localhost3", 11211);
$memcache->addServer("localhost4", 11211);

if ($memcache) {
	$init = 100;
	$val = 1;

	$key = "foo";
	$shardKey = "foo";
	$result = $memcache->setByKey($key, $init, 0, 9999, 0, $shardKey);
	echo "key '$key' shardKey '$shardKey' result '$result' val: '$init'\n";
	$result = $memcache->decrementByKey($key, $shardKey, $val);
	echo "key '$key' shardKey '$shardKey' result '$result' val: '$val'\n";
	$result = $memcache->decrementByKey($key, $shardKey, $val);
	echo "key '$key' shardKey '$shardKey' result '$result' val: '$val'\n";
	$result = $memcache->decrementByKey($key, $shardKey, $val);
	echo "key '$key' shardKey '$shardKey' result '$result' val: '$val'\n";

	$key = "foo";
	$shardKey = "123";
	$result = $memcache->setByKey($key, $init, 0, 9999, 0, $shardKey);
	echo "key '$key' shardKey '$shardKey' result '$result' val: '$init'\n";
	$result = $memcache->decrementByKey($key, $shardKey, $val);
	echo "key '$key' shardKey '$shardKey' result '$result' val: '$val'\n";
	$result = $memcache->decrementByKey($key, $shardKey, $val);
	echo "key '$key' shardKey '$shardKey' result '$result' val: '$val'\n";
	$result = $memcache->decrementByKey($key, $shardKey, $val);
	echo "key '$key' shardKey '$shardKey' result '$result' val: '$val'\n";
} else {
	echo "Connection failed\n";
}
?>

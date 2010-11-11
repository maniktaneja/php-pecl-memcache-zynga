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
	$timeout = 1209600;

	$val = "value1";
	$key = "foo";
	$flag = false;

	$result = $memcache->set($key, $val, $flag, $timeout);
	echo "key '$key' result '$result' val: '$val' flag '$flag' timeout '$timeout'\n";

	$val = "value2";
	$key = "foo";
	$flag = false;

	$result = $memcache->set($key, $val, $flag, $timeout);
	echo "key '$key' result '$result' val: '$val' flag '$flag' timeout '$timeout'\n";

	$val = "value3";
	$key = "fa;dfa;dfa;sdf;safoo";
	$flag = false;

	$result = $memcache->set($key, $val, $flag, $timeout);
	echo "key '$key' result '$result' val: '$val' flag '$flag' timeout '$timeout'\n";
} else {
	echo "Connection failed\n";
}
?>

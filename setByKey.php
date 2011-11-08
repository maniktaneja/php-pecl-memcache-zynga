<?php

require_once("init.php");

$memcache = new memcache;

$memcache->debug(DEBUG);
$memcache->addServer("localhost", 11211);

if ($memcache) {
	$timeout = 1209600;
	$cas = 0;

	$val = "value1";
	$key = "foo";
	$flag = false;
	$shardKey = "foo";

	$result = $memcache->setByKey($key, $val, $flag, $timeout, $cas, $shardKey);
	echo "key '$key' shardKey '$shardKey' result '$result' val: '$val' flag '$flag' timeout '$timeout' cas '$cas'\n";

	$val = "value2";
	$key = "foo";
	$flag = false;
	$shardKey = "123";

	$result = $memcache->setByKey($key, $val, $flag, $timeout, $cas, $shardKey);
	echo "key '$key' shardKey '$shardKey' result '$result' val: '$val' flag '$flag' timeout '$timeout' cas '$cas'\n";

	$val = "value3";
	$key = "fa;dfa;dfa;sdf;safoo";
	$flag = false;
	$shardKey = "123";

	$result = $memcache->setByKey($key, $val, $flag, $timeout, $cas, $shardKey);
	echo "key '$key' shardKey '$shardKey' result '$result' val: '$val' flag '$flag' timeout '$timeout' cas '$cas'\n";
} else {
	echo "Connection failed\n";
}
?>

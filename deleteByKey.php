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
	$time = 0;

	$key = "foo";
	$shardKey = "foo";

	$result = $memcache->deleteByKey($key, $shardKey, $time);
	echo "key '$key' shardKey '$shardKey' result '$result' time '$time'\n";

	$key = "foo";
	$shardKey = "123";

	$result = $memcache->deleteByKey($key, $shardKey, $time);
	echo "key '$key' shardKey '$shardKey' result '$result' time '$time'\n";

	$key = "fa;dfa;dfa;sdf;safoo";
	$shardKey = "123";

	$result = $memcache->deleteByKey($key, $shardKey, $time);
	echo "key '$key' shardKey '$shardKey' result '$result' time '$time'\n";
} else {
	echo "Connection failed\n";
}
?>

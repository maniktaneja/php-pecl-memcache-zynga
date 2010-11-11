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
	$keys = array(
		"foo" => "foo",
		"foo1" => "123",
		"foo2" => "123",
		"foo3" => "123",
		"foo4" => "123"
	);

	echo "keys " . print_r($keys, true);

	$result = $memcache->deleteMultiByKey($keys);

	echo "result " . print_r($result, true);
} else {
	echo "Connection failed\n";
}
?>

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
	$values = array();
	$keys = array(
		"foo" => "123",
		"test" => "123",
		"brent" => "567"
	);

	$status = $memcache->getMultiByKey($keys);

	echo "status " . print_r($status, true);
	echo "keys " . print_r($keys, true);
} else {
	echo "Connection failed\n";
}
?>

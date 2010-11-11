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
		"foo" => array(
			"value" => "value",
			"shardKey" => "foo",
			"flag" => 0,
			"cas" => 0,
			"expire" => 1209600
		),
		"foo1" => array(
			"value" => "value1",
			"shardKey" => "123",
			"flag" => 0,
			"cas" => 0,
			"expire" => 1209600
		),
		"foo2" => array(
			"value" => "value2",
			"shardKey" => "123",
			"flag" => 0,
			"cas" => 0,
			"expire" => 1209600
		),
		"foo3" => array(
			"value" => "value3",
			"shardKey" => "123",
			"flag" => 0,
			"cas" => 0,
			"expire" => 1209600
		),
		"foo4" => array(
			"value" => "value4",
			"shardKey" => "123",
			"flag" => 0,
			"cas" => 0,
			"expire" => 1209600
		)
	);

	echo "keys " . print_r($keys, true);

	$result = $memcache->addMultiByKey($keys);

	echo "result " . print_r($result, true);
} else {
	echo "Connection failed\n";
}
?>

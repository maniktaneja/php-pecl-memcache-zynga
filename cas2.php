<?php

require_once("init.php");

$memcache = new memcache;

$memcache->debug(DEBUG);
$memcache->addServer("localhost", 11211);
$memcache->addServer("localhost1", 11211);

if ($memcache) {
	$key = "123";
	$memcache->set($key, "TestValue1");
	
    $test = $memcache->get($key, $flag, $oldcas);
	echo "key '$key' cas '$cas' \n";
    $cas = $oldcas;

	$out = $memcache->cas($key, "testValue", $flag, 0, $cas);
	echo "key '$key' new cas '$cas' \n";

} else {
	echo "Connection failed\n";
}
?>

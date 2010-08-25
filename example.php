<?php

ini_set("memcache.proxy_enabled", true);
ini_set("memcache.proxy_host", "localhost:11311");

$memcache = new memcache;

$memcache->addServer("localhost", 11211);


if ($memcache) {

    $get_array = array("k1","k2","k3","k4","k5");
    $memcache->get($get_array);
    $memcache->get($get_array);
    $memcache->get($get_array);
    $memcache->get($get_array);

    $memcache->getl("k1");
    /*
    $memcache->get("k1");

	$memcache->set("str_key", "String to store in memcached");
	$memcache->set("num_key", 123);

	$object = new StdClass;
	$object->attribute = 'test';
	$memcache->set("obj_key", $object);

	$array = Array('assoc'=>123, 345, 567);
	$memcache->set("arr_key", $array);

	var_dump($memcache->get('str_key'));
	var_dump($memcache->get('num_key'));
	var_dump($memcache->get('obj_key'));
    */
}
else {
	echo "Connection to memcached failed";
}
?>


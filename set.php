<?php

require_once("init.php");

$memcache = new memcache;

$memcache->debug(DEBUG);
$memcache->addServer("localhost", 11211);

if ($memcache) {
	$timeout = 1209600;

	$val = "irish is a such a rascala dog, we need to give this rascala dog lots and lots of beatings";
	$key = "foo";
	$flag = 2;


    $memcache->setproperty('EnableChecksum', true);

	$result = $memcache->set($key, $val, $flag, $timeout);
	echo "key '$key' result '$result' val: '$val' flag '$flag' timeout '$timeout'\n";

	$val = "irish is a such a rascala dog, we need to give her lots of beatings irish is a such a rascala dog, we need to give her lots of beatings";
	$key = "foo1";
	$flag = 2;

	$result = $memcache->set($key, $val, $flag, $timeout);
	echo "key '$key' result '$result' val: '$val' flag '$flag' timeout '$timeout'\n";

	$key = "foo2";
	$val = "irish is a such a rascala dog, we need to give her lots of beatings irish is a such a rascala dog, we need to give her lots of beatings irish is a such a rascala dog, we need to give her lots of beatings irish is a such a rascala dog, we need to give her lots of beatings";
	$flag = 2;

	$result = $memcache->set($key, $val, $flag, $timeout);
	echo "key '$key' result '$result' val: '$val' flag '$flag' timeout '$timeout'\n";

    $result = $memcache->get("foo");
    echo "key '$key' result '$result";

    $result = $memcache->get("foo1");
    echo "key '$key' result '$result";

    $result = $memcache->get("foo2", $flags);
    echo "key '$key' result '$result \n flags :: $flags" ;


    $result = $memcache->append("foo2", "rak rak rak");
    $result = $memcache->get("foo2");
    $result = $memcache->set("sh", "2", $flag, $timeout);
    $result = $memcache->increment("sh");

    $result = $memcache->get("sh");


} else {
	echo "Connection failed\n";
}
?>

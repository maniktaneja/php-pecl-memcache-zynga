<?php

require_once("init.php");

$memcache = new memcache;

$memcache->debug(DEBUG);
$memcache->addServer("localhost", 11211);
$memcache->addServer("localhost1", 11211);
$memcache->addServer("localhost2", 11211);
$memcache->addServer("localhost3", 11211);
$memcache->addServer("localhost4", 11211);

if($memcache) {
	$key = "foo";
        $test = $memcache->get($key);
        echo "key '$key' result '$test' \n";

        $key = "123";
        $test = $memcache->get($key);
        echo "key '$key' result '$test' \n";

        $key = "fa;dfa;dfa;sdf;safoo";
        $test = $memcache->get($key);
        echo "key '$key' result '$test' \n";

} else {
	echo "Connection failed\n";
}
?>

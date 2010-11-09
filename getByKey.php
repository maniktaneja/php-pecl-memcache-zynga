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
	$val = null;

	$key = "foo";
	$shardKey = "foo";
        $test = $memcache->getByKey($key, $shardKey, $val);
        echo "key '$key' shardKey '$shardKey' result '$test' val: '$val'\n";

        $key = "foo";
        $shardKey = "123";
        $test = $memcache->getByKey($key, $shardKey, $val);
        echo "key '$key' shardKey '$shardKey' result '$test' val: '$val'\n";

        $key = "fa;dfa;dfa;sdf;safoo";
        $shardKey = "123";
        $test = $memcache->getByKey($key, $shardKey, $val);
        echo "key '$key' shardKey '$shardKey' result '$test' val: '$val'\n";

} else {
	echo "Connection failed\n";
}
?>

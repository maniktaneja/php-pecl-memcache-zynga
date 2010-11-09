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
        $test = $memcache->get2($key, $val);
        echo "key '$key' result '$test' val: '$val'\n";

        $key = "123";
        $test = $memcache->get2($key, $val);
        echo "key '$key' result '$test' val: '$val'\n";

        $key = "fa;dfa;dfa;sdf;safoo";
        $test = $memcache->get2($key, $val);
        echo "key '$key' result '$test' val: '$val'\n";

} else {
	echo "Connection failed\n";
}
?>

--TEST--
check setByKey method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache();
$memcache->addServer("192.168.1.4",11211);

$res1 = $memcache->set("foo","value1",false,1209600,0,"foo");
$res2 = $memcache->set("foo","value2",false,1209600,0,"123");
$res3 = $memcache->set("fa;dfa;dfa;sdf;safoo","value3",false,1209600,0,"123");

echo (string)$res1 . PHP_EOL;
echo (string)$res2 . PHP_EOL;
echo (string)$res3 . PHP_EOL;
--EXPECT--
1
1
1
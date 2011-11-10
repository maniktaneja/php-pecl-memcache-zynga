--TEST--
check get method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache();
$memcache->addServer("192.168.1.4",11211);
$res1 = $memcache->get("foo");
$res2 = $memcache->get("123");
$res3 = $memcache->get("fa;dfa;dfa;sdf;safoo");

echo $res1 . PHP_EOL;
echo $res2 . PHP_EOL;
echo $res3 . PHP_EOL;
--EXPECT--
value

value3
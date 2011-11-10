--TEST--
check decByKey method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache();
$memcache->addServer("192.168.1.4",11211);
$init=100;
$step = 1;
$key="foo";
$shardKey="foo";
$memcache->setByKey($key,$init,0,9999,0, $shardKey);
echo $memcache->decrementByKey($key,$shardKey,$step);
echo $memcache->decrementByKey($key,$shardKey,$step);
echo $memcache->decrementByKey($key,$shardKey,$step);
$shardKey="123";
echo $memcache->decrementByKey($key,$shardKey,$step);
echo $memcache->decrementByKey($key,$shardKey,$step);
echo $memcache->decrementByKey($key,$shardKey,$step);
--EXPECT--
999897969594
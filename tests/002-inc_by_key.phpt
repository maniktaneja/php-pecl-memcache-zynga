--TEST--
check incByKey method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache();
$memcache->addServer("192.168.1.4",11211);
$val=1;
$key="foo";
$shardKey="foo";
$memcache->setByKey($key,$val,0,9999,0, $shardKey);
echo $memcache->incrementByKey($key,$shardKey,$val);
echo $memcache->incrementByKey($key,$shardKey,$val);
echo $memcache->incrementByKey($key,$shardKey,$val);
$shardKey="123";
echo $memcache->incrementByKey($key,$shardKey,$val);
echo $memcache->incrementByKey($key,$shardKey,$val);
echo $memcache->incrementByKey($key,$shardKey,$val);
--EXPECT--
234567
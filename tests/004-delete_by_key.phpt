--TEST--
check setMultiByKey method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache();
$memcache->addServer("192.168.1.4",11211);

$time = 0;

$key = "foo";
$shardKey = "foo";

$result = $memcache->deleteByKey($key, $shardKey, $time);
echo "key '$key' shardKey '$shardKey' result '$result' time '$time'\n";

$key = "foo";
$shardKey = "123";

$result = $memcache->deleteByKey($key, $shardKey, $time);
echo "key '$key' shardKey '$shardKey' result '$result' time '$time'\n";

$key = "fa;dfa;dfa;sdf;safoo";
$shardKey = "123";

$result = $memcache->deleteByKey($key, $shardKey, $time);
echo "key '$key' shardKey '$shardKey' result '$result' time '$time'\n";

--EXPECT--
key 'foo' shardKey 'foo' result '1' time '0'
key 'foo' shardKey '123' result '' time '0'
key 'fa;dfa;dfa;sdf;safoo' shardKey '123' result '1' time '0'

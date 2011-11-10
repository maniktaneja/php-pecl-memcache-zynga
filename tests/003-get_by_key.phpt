--TEST--
check setMultiByKey method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache();
$memcache->addServer("192.168.1.4",11211);

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
--EXPECT--
key 'foo' shardKey 'foo' result '1' val: 'value'
key 'foo' shardKey '123' result '1' val: 'value'
key 'fa;dfa;dfa;sdf;safoo' shardKey '123' result '1' val: 'value3'


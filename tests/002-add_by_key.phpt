--TEST--
check setByKey method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache();
$memcache->addServer("192.168.1.4",11211);

$timeout = 1209600;
$cas = 0;

$val = "value1";
$key = "foo";
$flag = false;
$shardKey = "foo";

$result = $memcache->addByKey($key, $val, $flag, $timeout, $cas, $shardKey);
echo "key '$key' shardKey '$shardKey' result '$result' val: '$val' flag '$flag' timeout '$timeout' cas '$cas'\n";

$val = "value2";
$key = "foo";
$flag = false;
$shardKey = "123";

$result = $memcache->addByKey($key, $val, $flag, $timeout, $cas, $shardKey);
echo "key '$key' shardKey '$shardKey' result '$result' val: '$val' flag '$flag' timeout '$timeout' cas '$cas'\n";

$val = "value3";
$key = "fa;dfa;dfa;sdf;safoo";
$flag = false;
$shardKey = "123";

$result = $memcache->addByKey($key, $val, $flag, $timeout, $cas, $shardKey);
echo "key '$key' shardKey '$shardKey' result '$result' val: '$val' flag '$flag' timeout '$timeout' cas '$cas'\n";
--EXPECT--
key 'foo' shardKey 'foo' result '1' val: 'value1' flag '' timeout '1209600' cas '0'
key 'foo' shardKey '123' result '' val: 'value2' flag '' timeout '1209600' cas '0'
key 'fa;dfa;dfa;sdf;safoo' shardKey '123' result '1' val: 'value3' flag '' timeout '1209600' cas '0'


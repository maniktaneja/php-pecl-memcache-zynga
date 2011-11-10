--TEST--
check setMultiByKey method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache();
$memcache->addServer("192.168.1.4",11211);

$keys = array(
        "foo" => "foo",
        "foo1" => "123",
        "foo2" => "123",
        "foo3" => "123",
        "foo4" => "123"
);

echo "keys " . print_r($keys, true);

$result = $memcache->deleteMultiByKey($keys);

echo "result " . print_r($result, true);

--EXPECT--
keys Array
(
    [foo] => foo
    [foo1] => 123
    [foo2] => 123
    [foo3] => 123
    [foo4] => 123
)
result Array
(
    [foo] => 
    [foo1] => 1
    [foo2] => 1
    [foo3] => 1
    [foo4] => 1
)


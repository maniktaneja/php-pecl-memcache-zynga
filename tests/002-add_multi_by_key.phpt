--TEST--
check setByKey method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache();
$memcache->addServer("192.168.1.4",11211);
$keys = array(
        "foo" => array(
                "value" => "value",
                "shardKey" => "foo",
                "flag" => 0,
                "cas" => 0,
                "expire" => 1209600
        ),
        "foo1" => array(
                "value" => "value1",
                "shardKey" => "123",
                "flag" => 0,
                "cas" => 0,
                "expire" => 1209600
        ),
        "foo2" => array(
                "value" => "value2",
                "shardKey" => "123",
                "flag" => 0,
                "cas" => 0,
                "expire" => 1209600
        ),
        "foo3" => array(
                "value" => "value3",
                "shardKey" => "123",
                "flag" => 0,
                "cas" => 0,
                "expire" => 1209600
        ),
        "foo4" => array(
                "value" => "value4",
                "shardKey" => "123",
                "flag" => 0,
                "cas" => 0,
                "expire" => 1209600
        )
);

echo "keys " . print_r($keys, true);

$result = $memcache->addMultiByKey($keys);

echo "result " . print_r($result, true);
--EXPECT--
keys Array
(
    [foo] => Array
        (
            [value] => value
            [shardKey] => foo
            [flag] => 0
            [cas] => 0
            [expire] => 1209600
        )

    [foo1] => Array
        (
            [value] => value1
            [shardKey] => 123
            [flag] => 0
            [cas] => 0
            [expire] => 1209600
        )

    [foo2] => Array
        (
            [value] => value2
            [shardKey] => 123
            [flag] => 0
            [cas] => 0
            [expire] => 1209600
        )

    [foo3] => Array
        (
            [value] => value3
            [shardKey] => 123
            [flag] => 0
            [cas] => 0
            [expire] => 1209600
        )

    [foo4] => Array
        (
            [value] => value4
            [shardKey] => 123
            [flag] => 0
            [cas] => 0
            [expire] => 1209600
        )

)
result Array
(
    [foo] => 
    [foo1] => 1
    [foo2] => 1
    [foo3] => 1
    [foo4] => 1
)

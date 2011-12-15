--TEST--
check setMultiByKey method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache();
$memcache->addServer("localhost",11211);

$values = array();
$keys = array(
        "foo" => "123",
        "test" => "123",
        "brent" => "567"
);

$status = $memcache->getMultiByKey($keys);

echo  $status["foo"]["shardKey"] . PHP_EOL;
echo  $status["foo"]["status"] . PHP_EOL;
echo  $status["foo"]["value"] . PHP_EOL;

echo  $status["test"]["shardKey"] . PHP_EOL;
echo  $status["test"]["status"] . PHP_EOL;
echo  $status["test"]["value"] . PHP_EOL;

echo  $status["brent"]["shardKey"] . PHP_EOL;
echo  $status["brent"]["status"] . PHP_EOL;
echo  $status["brent"]["value"] . PHP_EOL;
--EXPECT--
123
1
value
123
1

567
1


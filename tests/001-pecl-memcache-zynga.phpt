--TEST--
Check for php-pecl-zynga-memcache presence
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
echo 'php-pecl-zynga-memcache is available';
--EXPECT--
php-pecl-zynga-memcache is available
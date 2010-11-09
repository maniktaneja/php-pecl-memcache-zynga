<?php

ini_set("memcache.proxy_enabled", 1);
ini_set("memcache.proxy_host","unix:///var/run/mcmux/mcmux.sock");
ini_set('memcache.allow_failover', 0);

define('DEBUG', true);
?>

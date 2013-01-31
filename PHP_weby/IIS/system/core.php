<?php

		$db = mysql_connect('localhost:/var/run/mysql/mysql.sock', 'xhalak00', 'pa2magon');
		if (!$db) die('nelze se pripojit '.mysql_error());
		if (!mysql_select_db('xhalak00', $db)) die('database neni dostupna '.mysql_error());
		mysql_query("SET CHARACTER SET utf8");
							
?>

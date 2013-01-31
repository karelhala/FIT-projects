<!-- navigation starts-->	
<div  id="nav">
	<ul>
		<?php

			if(!isset($current)) {
				$current = -1;
			}
			if($current == 2) {
				echo '<li id="current"><a href="index.php">Mé testy</a></li>';
			} else {
				echo '<li><a href="../testy/index.php">Mé testy</a></li>';
			}
			if($current == 1) {
				echo '<li id="current"><a href="index.php">Změna hesla</a></li>';
			} else {
				echo '<li><a href="../heslo/index.php">Změna hesla</a></li>';
			}
			
		?>
	</ul>
	<div id="logout">
		<p>
			<input class="button" type="button" onclick='location.href="../../logout.php"' value="Odhlásit"/>	
		</p>			
	</div>
<!-- navigation ends-->	
</div>

<?php
	$path = "../../logout.php";		
	$minPrava = 20;
	include("../../session.php");
?>

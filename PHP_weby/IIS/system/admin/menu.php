<!-- navigation starts-->	
<div  id="nav">
	<ul>
		<?php

			if(!isset($current)) {
				$current = -1;
			}

			if($current == 0) {
				echo '<li id="current"><a href="index.php">Zaměstnanci</a></li>';
			} else {
				echo '<li><a href="../uzivatel/index.php">Zaměstnanci</a></li>';
			}
			if($current == 2) {
				echo '<li id="current"><a href="index.php">Certifikáty</a></li>';
			} else {
				echo '<li><a href="../certifikat/index.php">Certifikáty</a></li>';
			}

			if($current == 3) {
				echo '<li id="current"><a href="index.php">Test</a></li>';
			} else {
				echo '<li><a href="../test/index.php">Test</a></li>';
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
	$minPrava = 0;
	include("../../session.php");
?>

<?php
	session_start();

	if( ! isset($_SESSION['login'])) {
		header("Location: ".$path);
	}

	session_regenerate_id();

	if( ! isset($minPrava)) {
		$minPrava = 0;
	}

	//oznámení o nedostatečných právech
	if($_SESSION['prava'] > $minPrava) {
	?>
		<div id="content">
			<div id="main">
				<p>Nedostatečná práva</p>
			</div>
		</div>
	<?php include("footer.php"); ?>
	</div></body></html>
	<?php 
		die();
	}
?>		

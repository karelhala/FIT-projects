<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<script type="text/javascript" src="../../js/jquery-1.5.2.min.js"></script>
	<script type="text/javascript" src="../../js/flexigrid.pack.js"></script>

	<link rel="stylesheet" href="../../images/flexgrid.css" type="text/css" />
	<?php include("../../head.php"); ?>	
	<?php include("../../core.php"); ?>
</head>
<body>

	<?php
		//include("post-json.php");
	?>
				
	<!-- wrap starts here -->
	<div id="wrap">
			
		<!-- navigační lišta-->
		<?php 
			$current = 3;
			include("../menu.php"); 
		?>		

		<!-- content starts -->
		<div id="content">
		
			<div id="main">

				<table id="flexme" style="display: none"></table>

				<script type="text/javascript">

					$("#flexme").flexigrid({
						url : 'post-json.php',
						dataType : 'json',
						colModel : [ {
							display : 'ID',
							name : 'ID',
							width : 20,
							sortable : true,
							align : 'center',
							process: process
						},   {
							display : 'Letadlo',
							name : 'typ',
							width : 70,
							sortable : true,
							align : 'left',
							sortable : true,
							process: process
						},  {
							display : 'Sériové číslo',
							name : 'seriovec',
							width : 80,
							align : 'left',
							sortable : true,
							process: process
						},  {
							display : 'Test',
							name : 'test',
							width : 200,
							align : 'left',
							process: process
						}, {
							display : 'Datum zadání',
							name : 'Datum_zadani',
							width : 75,
							sortable : true,
							align : 'center',
							hide : true,
							process: process
						}, {
							display : 'Přiřazeno',
							name : 'login_technika',
							width : 50,
							sortable : true,
							align : 'center',
							process: process
						} ],
						searchitems : [
							{display: 'ID', name : 'p.ID'},
							{display: 'Letadlo', name : 'typ'},
							{display: 'Sériové číslo', name : 'seriovec'},
						],
						sortname : "login_technika",
						sortorder : "asc",
						usepager : true,
						useRp : true,
						rp : 15,
						showTableToggleBtn : true,
						width : 850,
						height : 300
					});

					function process(celDiv, id){
				        $( celDiv ).click( function() {
					       window.location.replace("system.php?id="+id);
					    });
					}

				</script>
			<!-- main ends -->	
			</div>

		<!-- content ends-->	
		</div>
			
		<!-- zápatí lišta-->
		<?php include("../../footer.php"); ?>


	<!-- wrap ends here -->
	</div>
</body>
</html>
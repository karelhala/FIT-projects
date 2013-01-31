<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>	
<link rel="stylesheet" href="images/style.css" type="text/css" />
<?php include("head.php"); ?>	
</head>

<body>

	<div id="login">

		<form action="login.php" method="post">
			<table>
			<?php include("index_msg.php"); ?>	
			<tr>
				<td align="right">Uživatelské jméno : </td>
				<td><input type="text" name="login"/></td>
			</tr>
			<tr>
				<td align="right">Heslo : </td>
				<td><input type="password" name="pass"/></td>
			</tr>
			<tr>
				<td colspan="2" align="center"><input class="button" type="submit" value="Přilásit"/></td>
			</tr>
			</table>
		</form>
	</div>

</body>
</html>

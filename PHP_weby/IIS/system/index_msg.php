<?php
	if (isset($_GET['bad'])) {
?>
		<tr>
			<td colspan="2" align="center"><span style="color:red">Špatný login nebo heslo.</span></td>
		</tr>
<?php				
	} elseif (isset($_GET['logout'])) {
?>
		<tr>
			<td colspan="2" align="center">Byl jste odhlášen.</td>
		</tr>
<?php
	}
?>
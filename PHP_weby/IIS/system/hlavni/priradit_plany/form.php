<?php
	header("Content-Type: text/html; charset=utf-8");	
	include("../../core.php");

	//CAST ZPRACOVAVAJICI DATA Z FORMULARE
	if(isset($_POST['action'])) {

		//odtraneni zaznamu
		if($_POST['action'] == 'Odstranit'){

			$result = mysql_query('DELETE FROM Plany WHERE ID ="'.$_POST['id_pl'].'"');
			header("Location: index.php?msg=2");	

		//vytvoření záznamu
		} else if($_POST['action'] == 'Vytvořit'){

			$date = date("Y-m-d");

			//přidání plánu
			$result = mysql_query('INSERT INTO Plany SET Hotovo=0, Datum_zadani="'.$date.'", ID_test="'.$_POST['test'].'", ID_letadlo="'.$_POST['letadlo'].'"');
			$id = mysql_insert_id();

			if($result) {
				//přiřazení techniků k plánu
				if(isset($_POST['technici'])) {
					foreach($_POST['technici'] as $technik) {
						$result = mysql_query('INSERT INTO Ma_provest SET login_technika="'.$technik.'", ID_Plany="'.$id.'"');
					}
				}
				header("Location: index.php?msg=1");
			} else {
				header("Location: index.php?msg=3");
			}

		} elseif ($_POST['action'] == 'Uložit') {
			$result = mysql_query('UPDATE Plany SET ID_test="'.$_POST['test'].'" WHERE ID ="'.$_POST['id_pl'].'"');
			$result = mysql_query('DELETE FROM Ma_provest WHERE ID_Plany="'.$_POST['id_pl'].'"');

			//přiřazení techniků k plánu
			if(isset($_POST['technici'])) {
				foreach($_POST['technici'] as $technik) {
					$result = mysql_query('INSERT INTO Ma_provest SET login_technika="'.$technik.'", ID_Plany="'.$_POST['id_pl'].'"');
				}
			}

			header("Location: index.php?msg=4");
		}


	//CAST VYTVAREJICI FORMULAR
	} else {

		if( ! isset($id)) {
			$id = -1;
			$id_let = -1;
			$id_test = -1;
			$tech = array();
		} else {
			$result = mysql_query('SELECT * FROM Plany p LEFT JOIN Ma_provest m ON p.ID = m.ID_Plany WHERE p.ID = "'.$id.'"');
			$tech = array();

			while ($row = mysql_fetch_array($result)) {
				$id_test = $row['ID_test'];
				$id_let = $row['ID_letadlo'];				
				$tech[] = $row['login_technika'];
			}
		}

?>
		<form action="form.php" method="post">
		<table>
			<tr>
				<td>Letadlo: </td>
				<td>
					<select name="letadlo" size="1">
					<?php

						$result = mysql_query('SELECT * FROM Letadlo');

						while ($row = mysql_fetch_array($result)) {
							echo '<option value="'.$row['ID'];
							if($row['ID'] == $id_let) {
								echo '" selected="';
							}
							echo '">'.$row['typ'].", ".$row['seriovec'] ;
						}
					?>
					</select>*
				</td>
				<td>Test: </td>
				<td>
					<select id="testy" name="test" size="1" onchange="refresh()">
					<?php
						if($vytvor) {
							echo '<option id="emtyopt" value="-1">';
						}

						$result = mysql_query('SELECT * FROM Test');

						while ($row = mysql_fetch_array($result)) {
							echo '<option value="'.$row['ID'];
							if($row['ID'] == $id_test) {
								echo '" selected="';
							}
							echo '">'.$row['popis'];
						}
					?>
					</select>*
				</td>
			</tr>
			<tr>
				<td>Technici:</td>
				<td colspan="2">
					<select id="technici" name="technici[]" size="10" multiple="" width="300"  style="width: 300px">
						<?php
							if(!$vytvor) {
								include('technici.php');
							}
						?>
					</select>
				</td>
				<script>
					function refresh() {

						//odstranění úvodní prázdné položky
						elem=document.getElementById("emtyopt");
						if(elem != null) {
							elem.parentNode.removeChild(elem);
						}

						var e = document.getElementById("testy");
						var id = e.options[e.selectedIndex].value;
						var ajax = new XMLHttpRequest();
						ajax.onreadystatechange = function() {
							if (ajax.readyState == 4) {
								 document.getElementById("technici").innerHTML = ajax.responseText;
							}
						};
						ajax.open("GET", "technici.php?id="+id, true);
						ajax.send(null);
					}

				</script>
			</tr>
		</table>


		<table>
			<tr>
				<?php if($vytvor) { ?>
					<td><input class="button" type="button" onClick="location.href='index.php'" value="Zpět"/></td>
					<td><input class="button" name="action" type="submit" value="Vytvořit"/></td>
				<?php } else { ?>
					<td><input class="button" type="button" onClick="location.href='grid.php'" value="Zpět"/></td>
					<td><input class="button" name="action" type="submit" value="Uložit"/></td>
					<td><input class="button" name="action" type="submit" value="Odstranit"/></td>
				<?php } ?>
			</tr>
		</table>
		<input name="id_pl" type="hidden" value="<?php echo $id; ?>">
		</form>
<?php	} ?>

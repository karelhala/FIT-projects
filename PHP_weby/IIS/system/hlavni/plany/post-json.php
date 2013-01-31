<?php
$page = isset($_POST['page']) ? $_POST['page'] : 1;
$rp = isset($_POST['rp']) ? $_POST['rp'] : 10;
$sortname = isset($_POST['sortname']) ? $_POST['sortname'] : 'p.ID';
$sortorder = isset($_POST['sortorder']) ? $_POST['sortorder'] : 'desc';
$query = isset($_POST['query']) ? $_POST['query'] : false;
$qtype = isset($_POST['qtype']) ? $_POST['qtype'] : false;

//tabulka z keré se vyhledává
$table = "Plany p LEFT JOIN  Ma_provest m ON m.ID_Plany = p.ID LEFT JOIN Letadlo l ON p.ID_letadlo = l.ID LEFT JOIN Technik t ON m.login_technika = t.login LEFT JOIN Test tt ON p.ID_test = tt.ID";
$where = "";


$usingSQL = true;
function runSQL($rsql) {

	$db['default']['hostname'] = "localhost:/var/run/mysql/mysql.sock";
	$db['default']['username'] = 'xhalak00';
	$db['default']['password'] = "pa2magon";
	$db['default']['database'] = "xhalak00";

	$active_group = 'default';

	$base_url = "http://".$_SERVER['HTTP_HOST'];
	$base_url .= str_replace(basename($_SERVER['SCRIPT_NAME']),"",$_SERVER['SCRIPT_NAME']);

	$connect = mysql_connect($db[$active_group]['hostname'],$db[$active_group]['username'],$db[$active_group]['password']) or die ("Error: could not connect to database");
	$db = mysql_select_db($db[$active_group]['database']);
	mysql_query("SET CHARACTER SET utf8");

	$result = mysql_query($rsql) or die ($rsql);
	return $result;
	mysql_close($connect);
}

function countRec($fname,$tname) {
	$sql = "SELECT count($fname) FROM $tname ";
	$result = runSQL($sql);
	while ($row = mysql_fetch_array($result)) {
		return $row[0];
	}
}

$sort = "ORDER BY $sortname $sortorder";
$start = (($page-1) * $rp);

$limit = "LIMIT $start, $rp";

if ($query) {
	if($qtype == 'Hotovo') {
			if($query == "Ne") {
				$query = 0;
			} else {
				$query = 1;
			}
		$where = " WHERE Hotovo = $query ";
	} else {
		$where = " WHERE $qtype LIKE '%".$query."%' ";
	}
}

$sql = "SELECT p.ID, p.vysledek, p.Datum_zadani, p.Datum_ukonceni, l.typ, m.login_technika, p.Hotovo, t.jmeno, t.prijmeni, l.seriovec, tt.popis AS test_pop  FROM $table $where $sort $limit";
$result = runSQL($sql);

$total = countRec("p.ID","$table $where");



header("Content-type: application/json");
$jsonData = array('page'=>$page,'total'=>$total,'rows'=>array());

while ($row = mysql_fetch_assoc($result)) {

		$hotovo = ($row['Hotovo'] == 0) ? "Ne" : "Ano";
		$datum = ($row['Datum_ukonceni'] == '0000-00-00') ? "" : $row['Datum_ukonceni'];


		$entry = array('id'=>$row['ID'],
			'cell'=>array(
				'ID'=>$row['ID'],
				'vysledek'=>$row['vysledek'],
				'seriovec'=>$row['seriovec'],
				'Datum_zadani'=> $row['Datum_zadani'],
				'Datum_ukonceni'=>$datum,
				'Hotovo'=> $hotovo,
				'typ' => $row['typ'],
				'login_technika' => $row['login_technika'],
				'jmeno'=>$row['prijmeni']." ".$row['jmeno'],
				'test' => $row['test_pop'],
			),
		);
		$jsonData['rows'][] = $entry;
    }
echo json_encode($jsonData);

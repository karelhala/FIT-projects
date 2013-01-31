"Program Petri´s nets slouží pro práci s petriho sítěmi. Tyto sítě je
možné vytváře, upravovat a ukládat a to jak lokálně, tak také na
serveru.

Vytváření síťí
Jednotlivé sítě se zobrazují v tabech a je možné pracovat s více
sítěmi najednou. Síť je tvořena z míst, přechodů a hran.
Místa se vytváří pomocí tlačítka Place a kliknutím do oblasti, kde se
má místo vytvořit. Poté vyskočí dialogové okno, kde je možné zadat
hodnoty pro dané místo.
Tyto hodnoty je možné později změnit Stejně tak funguje vytváření
přechodů pomocí tlačítka Transition.
Pokud chcete spojit nějaké místo s přechodem, tak je to možné pomocí
tlačítka Join. Klikněte na tlačítko a klikněte na vstupní prvek (ať už
místo nebo hranu) a na výstupní prvek.
Vyskočí dialogové okno, které bude požadavat označení pro dané místo v
daném přechodě a pod tímto označením potom můžete v přechodě u
podmínek a výstupů na dané místo odkazovat.
Automaticky se vytvoří hrana a šipka ukazuje, zda místo vstupuje (nebo
vystupuje) do (nebo z) přechodu.
Jakmile máte spojeno nějaké místo s přechodem a označeny hrany, tak
můžete upravit přechod a přidat do něj podmínky nebo výstupy.
Pokud chcete nějaké prvky upravovat, tak je nutné kliknout na tlačítko
Place nebo Transition a kliknout na daný prvek.
Pokud máte tedy vstupní místa do přechodu, tak můžete vytvořit
podmínky jako x > 0 (kde x je označení hrany) a jednotlivé podmínky
oddělit &.
Stejně tak pokud máte výstupní místa z přechodu, tak můžete vytvořit
výstupy typu a = x - 2, kde a je označení hrany do výstupního místa a
x je označení hrany vstupního místa.
Obsahy prvků je možné kdykoliv měnit, stejně tak je možné pomocí
tlačítka delete smazat určité prvky nebo pomocí Clear.

Ukládání síťí
Sítě je možné ukládat jak lokálně pomocí položek Load a Save pod
položkou file nebo po přihlášení k serveru položkami load a save pod
položkou server.
Sítě se ukládají ve formátu xml. Pokud síť uložíme na serveru, tak
můžeme síť uložit do nového souboru nebo vybrat již existující
souboru.
Pokud vybereme existující, tak se sít uloží jako další verze dané
sítě. Sítě je možné na serveru vyhledávat pomocí názvu nebo uživatele,
který je vytvořil.

Simulace síťí
Simulace síťí probíhá na serveru a operace jsou přístupné pod položkou
server. Nejdříve je potřeba se na serveru registrovat pomocí jména a
hesla a poté se pomocí těchto údajů přihlásit.
Simulaci je možné provádět po jednotlivých krocích pomocí simulate one
step nebo provést celou simulaci naráz pomocí simulate.
Pokud jsou na serveru uložené nějaké sítě, tak je možné jich simulovat
více najednou pomocí Run."


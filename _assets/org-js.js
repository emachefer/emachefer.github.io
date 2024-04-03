// Variables used in all document

onload = () => {
    const d = new Date();
    const URL = window.location.href;
    console.log(`Loaded "${URL}" on : ${d.toDateString()}`);
    // Show :prof: if #prof in url
    const anchor = URL.split('#')[1];
    const out2 = document.getElementsByClassName("outline-2");
    const out3 = document.getElementsByClassName("outline-3");
    if (anchor === "prof") {
	for (el of out2) {
	    const prof = el.getElementsByClassName("prof");
	    if (prof !== null) {
		el.style["display"] = "block";
	    }
	}
	for (el of out3) {
	    const prof = el.getElementsByClassName("prof");
	    if (prof !== null) {
		el.style["display"] = "block";
	    }
	}
    }
    for (el of out2) {
	const h2 = el.getElementsByTagName("h2")[0];
	// FIXME peut import si h2 contient ou non <span class="done">
	// le test est considéré vrai
	if (h2.getElementsByClassName("done") !== null) {
	    el.classList.toggle("hidden");
	}
    }
    // Load dirs for navbar
    // TODO recreate json on index export
    fetch('/_assets/dirs.json')
	.then((response) => response.json())
	.then((json) => console.log(json));
};


onscroll = (event) => {
    const scrolled = window.scrollY;
    const totHeight = document.body.scrollHeight;
    const tocTitle=document.getElementById("table-of-contents").getElementsByTagName("h2")[0];
    const tocHeight = tocTitle.style.getPropertyValue("height");
    tocTitle.style.boxShadow = `inset 0 ${25*scrolled/(totHeight)}vb red`;
};


function isEmpty(node) {
  return node.textContent.trim() === "";
}


// TODO add event listener instead?
onclick = (event) => {
    let cible =  event.target;
    const check = (tg) => {
	// Si click sur titres, il faut cacher le reste
	return (tg.tagName == "H2" || tg.tagName == "H3" || tg.tagName == "H4");
    }

    if (check(cible)) {
	cible = cible.parentNode; // Cible seulement outline-[:digits:] pour classe "hidden"
	cible.classList.toggle("hidden");
    }
};


// Fonction pour changer la couleur, fonctionne avec id de input color :
// <input name="bgColor" id="bgColor" type="color" onchange="selectColor('bgColor', 'backgroundColor')" />
function selectColor(id, element) {
    let color = document.getElementById(id);
    let content = document.getElementById('content');
    content.style[element] = color.value;
}

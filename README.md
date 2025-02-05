# Moteur-physique-8INF935-
Projet du cours math physique pour le jeu vidéo , création d'un moteur physique "réaliste"
![Screenshot](https://github.com/oeodk/Moteur-physique-8INF935-/blob/main/screenshot.png)


<h3>But</h3>
<p>Créer un moteur physique dont les objets sont des corps rigides
et dont la résolution de collisions se fait de manière itérative à
l’aide d’impulsions. Ce but sera atteint graduellement au fil de
la session. La conception se fera en quatre (4) phases itératives : 
</p>
<ul id="yui_3_17_2_1_1724957392104_387">
	<li>
<p>Phase 1&nbsp;: construction d’un moteur élémentaire de
	gestion de particules;</p>
	</li><li>
<p>Phase 2&nbsp;: gestion d’objets formés de plusieurs
	masses par l’ajout de différentes forces et contraintes;</p>
	</li><li>
<p>Phase 3&nbsp;: gestion des corps rigides par l’ajout de
	la physique rotationnelle;</p>
	</li><li>
<p>Phase 4&nbsp;: ajout d’un système élémentaire de détection et résolution de collisions.</p>
</li></ul>
<h3>Exigences</h3>
<p>Le moteur doit être codé en C++ et utiliser le paradigme de programmation orienté objet. Bien que le rendu graphique ne soit pas priorisé, vous devez obligatoirement incorporer une interface graphique à votre projet qui permet de bien visualiser les différents principes physique à l’oeuvre. Il est fortement conseillé d’utiliser la librairie présentée dans la semaine 2.</p><p>Le projet se fait en équipe d’au plus quatre (4) personnes. Tout plagiat entraînera automatique la note de zéro (0).</p>
<h3>Pondération et évaluation</h3>
<p>Chaque phase sera évaluée indépendamment lors de quatre (4) présentations orales se déroulant au bureau du professeur . Un seul membre de l’équipe est présent à chaque présentation. Tous les membres de l’équipe doivent obligatoirement présenter au moins une fois. Le code source sera également évalué et devra répondre aux critères de bonnes pratiques de programmation : code lisible, structuré et bien commenté.</p>
<p>La pondération de chaque phase est de 25%.</p>
<h3>Livrable et échéanciers</h3>
<p>Pour chaque phase, vous devez remettre&nbsp;:</p>
<ul>
	<li>
<p>Un document Identification.txt contenant vos noms, prénoms
	et codes permanents</p>
	</li><li>
<p>Vos fichiers sources;</p>
	</li><li>
<p>Un fichier ReadMe.txt expliquant comment compiler et faire
	fonctionner votre programme;</p>
	</li><li>
<p>Tout autre fichier nécessaire à l’exécution de votre
	programme</p>
	</li><li>
<p>Un journal de développement, format PDF, d’environ une
	(1) à deux (2) pages expliquant les difficultés rencontrées, les
	astuces de programmation que vous avez appris ainsi que la
	justification des différents choix faits durant le développement.</p>
</li></ul>
<p>Ces documents doivent être placés dans un dossier et remis directement dans le dépôt Moodle approprié.</p>
<p>Les dates de remise sont indiquées dans le plan de cours.</p>
<h2 style="text-align: left;">Description des différentes phases
du projet</h2>
<p><span><span><span style=""><u><strong>
Phase
1</strong></u></span></span></span></p>
<p>Le but de cette phase est de créer un moteur physique simple de
gestion de particules à partir des notions vues aux semaines 1 à 4.
En particulier, vous devez&nbsp;:</p>
<ul>
	<li>
<p>Implémenter une classe Vecteur3D
	avec toutes les méthodes appropriées: norme, norme au carré, normalisation, multiplication par un scalaire, addition, soustraction, produit par composantes, produit scalaire, produit vectoriel, etc.</p>
	</li><li><p>Implémenter une classe de test ou des test unitaires pour la classe Vecteur3D.</p></li><li>
<p>Implémenter une classe Particule
	comprenant des accesseurs pour l’attribut InverseMasse.
		</p>
	</li><li>
<p>Implémenter un intégrateur (intégration d'Euler et/ou intégration de Verlet) afin de mettre à jour la
	position et la vélocité de chaque particule à chaque frame.
	Celui-ci peut être implémenté en tant que méthode void
	integrer(float temps) de la classe Particule.</p>
	</li><li>
<p>Réaliser un petit jeu de tir balistique où on peut
	choisir entre différents projectiles (e.g. balles, boulets, laser,
	boule de feu) et effectuer un tir. Chaque projectile doit avoir sa
	propre vélocité initiale et sa propre masse. Le frottement doit
	être négligeable (près de 1). La trajectoire des projectiles doit être clairement visible à l'écran</p>
</li></ul>Remarque: vous devez calculer la durée d'une frame et afficher la valeur en temps réel en mesurant la longueur de chaque frame afin de l'appliquer à la prochaine.<p><br></p><p>
</p><p><strong><u>
Phase
2</u></strong></p>
<p>Le but de cette phase est de poursuivre le développement du
moteur physique réalisé lors de la phase 1 en lui ajoutant les
notions vues au chapitres 4, 5 et 6. Ainsi, le moteur permettra la
gestion d’amas de particules. En particulier, vous devez&nbsp;:</p>
<ul>
	<li>
<p>Implémenter, à l’aide  d’une interface, des
	générateurs pour les forces suivantes&nbsp;: gravité, friction et
	ressorts. Notez que tous les types de ressorts vus en classe doivent
	être implémentés.</p>
	</li><li>
<p>Implémenter un système de détection de collisions. Ce
	système devra détecter les types de collisions suivants&nbsp;:
	interpénétrations, contacts au repos, tiges et câbles.<em> </em>
	</p>
	</li><li>
<p>Implémenter un système de résolution de collisions basé
	sur les impulsions.</p>
	</li><li>
<p>Réaliser un petit jeu où l’on déplace un amas de
	particule, i.e. <em>blob</em>. Chaque
	particule est représentée par un cercle. Les particules sont
	reliées entre elles en utilisant des ressorts classiques. Par
	contre, ces liens ont une limite d’élasticité au-delà de
	laquelle ils se comportent comme des câbles.</p>
	</li><li>
<p>Vous
	aurez des points boni si votre blob peut se séparer en plusieurs
	morceaux et se fusionner.</p>
</li><li><p>(FACULTATIF) Ajouter un HUD où l'on affiche en temps réel le nombre de particules attachées au Blob. Chaque nouvelle valeur rebondit avec un mouvement harmonique simple amorti lorsqu'elle apparait.</p></li></ul>
<p>Exemple de blob : le jeu <em>Loco
Roco</em> de SCE Japan Studio.</p>

<p></p>
<p><u>Suggestion d’implémentation</u>&nbsp;: construire une classe
World
représentant le <em>game
world</em> et qui
contient, entre autre chose, un std&nbsp;::vector
avec toutes les particules actives dans le jeu. Ensuite, à chaque
frame, pour chaque particule, appeler le générateur de forces,
intégrer et accumuler les contacts. Finalement, passer ces contacts
au système de résolution de collisions.</p>
<p><strong><u>Phase
3</u></strong><br></p>
<p>Le but de cette phase est de
spécialiser le moteur
physique des phases 1 et 2 par l’ajout de la physique
rotationnelle, implémentant ainsi
les
notions vues au chapitres 7, 8 et 9.
Ce faisant,
le moteur permettra la gestion des
corps rigides. Notez
toutefois que la gestion des collisions n’est pas à implémenter
dans cette phase. En
particulier, vous devez&nbsp;:</p>
<ul>
	<li>
<p>Implémenter
	une classe CorpsRigide
	avec les attributs et méthodes pertinentes.</p>
	</li><li>
<p>Implémenter les
	classes Matrix3,
	Matrix4
	et Quaternion
	avec les attributs et méthodes pertinentes.</p>
	</li><li><p>Implémenter une classe test ou des tests unitaires pour les classes Matrix3, Matrix4 et Quaternion.</p></li><li>
<p>Implémenter un
	intégrateur physique complet permettant la mise à jour des objets
	de type CorpsRigide.</p>
	</li><li>
<p>Réaliser un jeu de tir balistique 3D où des objets de forme irrégulières sont lancés en l’air en leur appliquant une force ailleurs que sur leur centre de masse. Le centre de masse
		ainsi que le mouvement de rotation doivent être clairement
		visibles.</p></li></ul>
<p><strong><u>
Phase
4</u></strong></p>
<p>Le but de cette phase est
d’ajouter au moteur
physique de la phase 3 un système simple de détection de
collisions, implémentant ainsi les notions vues aux chapitres 10 et
11. Ce faisant, le moteur permettra la détection de collisions entre
une boîte et un plan. Notez toutefois que vous n’avez pas à
implémenter un système complet de gestion de collisions.</p><p>Pour cette phase, vous devez ajouter un système de détection et résolution de collisions (phases élargie et restreintes, résolution à l'aide d'impulsions) à votre jeu de la phase 3. Plusieurs projectiles en forme de boîtes sont lancés et entrent en collisions entre eux. Chaque collision doit être détectée et résolue. La structure de partition de l'espace doit être visible en temps réel (soit directement dans le jeu, soit dans une autre fenêtre).</p>
<p>Pour ce faire,
vous devez,&nbsp;en
particulier:</p>
<ul>
	<li>
<p>implémenter un
	système de détection de collisions (phase élargie) à l’aide d'un octree ou d'un BSP tree. Utilisez une sphère comme volume englobant.</p>
	</li><li>
<p>implémenter un
	système de génération de collisions (phase restreinte) boîte-plan
	tel que vu en classe.</p>
	</li><li><p>résoudre les collisions avec des impulsions comme à la phase 2.</p></li><li>
<p>implémenter toutes
	les classes pertinentes à la réalisation de ce projet&nbsp;:
	ocTree,
	Contact,
	CollisionData,
	Primitive,
	Plane,
	Sphere,
	Box,
	etc.</p>
</li></ul><br><p></p><p></p><p></p></div></div></div>
                        
                        

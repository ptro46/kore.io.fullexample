create table magasin(
       idt         serial          PRIMARY KEY ,
       nom    	   text        	not null
);

create table rayon(
	idt         serial          PRIMARY KEY ,
	idt_magasin	integer			not null,
    nom		    text        	not null,
    nom_image	text			not null
);

ALTER TABLE ONLY rayon ADD CONSTRAINT fk_rayon_magasin
        FOREIGN KEY (idt_magasin) REFERENCES magasin(idt) ON DELETE CASCADE;

create table produit(
	idt         serial          PRIMARY KEY ,
	idt_rayon	integer			not null,
    nom		    text        	not null,
    nom_image	text			not null,
    prix		float			not null
) ;

ALTER TABLE ONLY produit ADD CONSTRAINT fk_produit_rayon
        FOREIGN KEY (idt_rayon) REFERENCES rayon(idt) ON DELETE CASCADE;

create table panier(
	idt         serial          PRIMARY KEY ,
	idt_magasin	integer			not null,
	username	text			not null
);

ALTER TABLE ONLY panier ADD CONSTRAINT fk_panier_magasin
        FOREIGN KEY (idt_magasin) REFERENCES magasin(idt) ON DELETE CASCADE;

create table panier_item(
	idt_panier	integer			not null,
	idt_produit	integer			not null,
    quantite	integer			not null
) ;

ALTER TABLE ONLY panier_item ADD CONSTRAINT fk_panieritem_panier
        FOREIGN KEY (idt_panier) REFERENCES panier(idt) ON DELETE CASCADE;

ALTER TABLE ONLY panier_item ADD CONSTRAINT fk_panieritem_produit
        FOREIGN KEY (idt_produit) REFERENCES produit(idt) ON DELETE CASCADE;


insert into magasin(nom) values('Lille');
insert into magasin(nom) values('Gourdon');

insert into rayon(idt_magasin,nom,nom_image) values(1,'Eaux','eau');
insert into rayon(idt_magasin,nom,nom_image) values(1,'Produits d''entretien','entretien');
insert into rayon(idt_magasin,nom,nom_image) values(1,'Epicerie','epicerie');
insert into rayon(idt_magasin,nom,nom_image) values(1,'Fruits','fruits');
insert into rayon(idt_magasin,nom,nom_image) values(1,'Légumes','legumes');
insert into rayon(idt_magasin,nom,nom_image) values(1,'Hi-Fi','hifi');
insert into rayon(idt_magasin,nom,nom_image) values(1,'Hygiène','hygiene');
insert into rayon(idt_magasin,nom,nom_image) values(1,'Papétrie','papetrie');
insert into rayon(idt_magasin,nom,nom_image) values(1,'Viandes','viandes');

insert into rayon(idt_magasin,nom,nom_image) values(2,'Eaux','eau');
insert into rayon(idt_magasin,nom,nom_image) values(2,'Produits d''entretien','entretien');
insert into rayon(idt_magasin,nom,nom_image) values(2,'Epicerie','epicerie');
insert into rayon(idt_magasin,nom,nom_image) values(2,'Fruits','fruits');
insert into rayon(idt_magasin,nom,nom_image) values(2,'Légumes','legumes');
insert into rayon(idt_magasin,nom,nom_image) values(2,'Hi-Fi','hifi');
insert into rayon(idt_magasin,nom,nom_image) values(2,'Hygiène','hygiene');
insert into rayon(idt_magasin,nom,nom_image) values(2,'Papétrie','papetrie');
insert into rayon(idt_magasin,nom,nom_image) values(2,'Viandes','viandes');

insert into produit(idt_rayon,nom,nom_image,prix) values(1,'Evian','evian',1.3);
insert into produit(idt_rayon,nom,nom_image,prix) values(1,'Hépar','hepar',1.24);

insert into produit(idt_rayon,nom,nom_image,prix) values(2,'Destop','destop',1);
insert into produit(idt_rayon,nom,nom_image,prix) values(2,'Lessive l''Arbre vert','lessive',3);
insert into produit(idt_rayon,nom,nom_image,prix) values(2,'Liquide Vaisselle Paic Citron','paic',1.5);

insert into produit(idt_rayon,nom,nom_image,prix) values(3,'Pâte Lustucru','lustucru',2.5);
insert into produit(idt_rayon,nom,nom_image,prix) values(3,'Pâtes Panzani','panzani',0.75);
insert into produit(idt_rayon,nom,nom_image,prix) values(3,'Douceur de légumes du soleil','soupe',4);

insert into produit(idt_rayon,nom,nom_image,prix) values(4,'1Kg de Bananes','banane',4);
insert into produit(idt_rayon,nom,nom_image,prix) values(4,'1Kg de Clémentines','clementine',3.1);
insert into produit(idt_rayon,nom,nom_image,prix) values(4,'1Kg de Pommes','pomme',1.56);

insert into produit(idt_rayon,nom,nom_image,prix) values(5,'1Kg de Carottes','carotte',2);
insert into produit(idt_rayon,nom,nom_image,prix) values(5,'1Kg de Tomates','tomate',1.03);
insert into produit(idt_rayon,nom,nom_image,prix) values(5,'3 Poivrons','poivron',0.65);

insert into produit(idt_rayon,nom,nom_image,prix) values(6,'Téléviseur 138 cm LED','televiseur',759);
insert into produit(idt_rayon,nom,nom_image,prix) values(6,'VideoProjecteur 1080p','videoprojecteur',1035);

insert into produit(idt_rayon,nom,nom_image,prix) values(7,'Shampoing qui lave','shampoing',3);
insert into produit(idt_rayon,nom,nom_image,prix) values(7,'Shampoing qui lisse','dessange',1.55);
insert into produit(idt_rayon,nom,nom_image,prix) values(7,'Shampoing qui sent bon','dove',4.65);

insert into produit(idt_rayon,nom,nom_image,prix) values(8,'Stylo Bic','bic',0.30);
insert into produit(idt_rayon,nom,nom_image,prix) values(8,'Cahier à carreaux','cahier',1.08);
insert into produit(idt_rayon,nom,nom_image,prix) values(8,'Papier blanc','ramette',3.54);

insert into produit(idt_rayon,nom,nom_image,prix) values(9,'Poulet roti','poulet',3.45);
insert into produit(idt_rayon,nom,nom_image,prix) values(9,'Roti de Porc','roti',2.54);
insert into produit(idt_rayon,nom,nom_image,prix) values(9,'Steak haché','steak',1.05);


insert into produit(idt_rayon,nom,nom_image,prix) values(10,'Evian','evian',1.3);

insert into produit(idt_rayon,nom,nom_image,prix) values(11,'Destop','destop',1);
insert into produit(idt_rayon,nom,nom_image,prix) values(11,'Lessive l''Arbre vert','lessive',3);

insert into produit(idt_rayon,nom,nom_image,prix) values(12,'Pâte Lustucru','lustucru',2.5);
insert into produit(idt_rayon,nom,nom_image,prix) values(12,'Pâtes Panzani','panzani',0.75);

insert into produit(idt_rayon,nom,nom_image,prix) values(13,'1Kg de Bananes','banane',4);
insert into produit(idt_rayon,nom,nom_image,prix) values(13,'1Kg de Clémentines','clementine',3.1);

insert into produit(idt_rayon,nom,nom_image,prix) values(14,'1Kg de Carottes','carotte',2);
insert into produit(idt_rayon,nom,nom_image,prix) values(14,'1Kg de Tomates','tomate',1.03);

insert into produit(idt_rayon,nom,nom_image,prix) values(15,'Téléviseur 138 cm LED','televiseur',759);

insert into produit(idt_rayon,nom,nom_image,prix) values(16,'Shampoing qui lave','shampoing',3);
insert into produit(idt_rayon,nom,nom_image,prix) values(16,'Shampoing qui lisse','dessange',1.55);

insert into produit(idt_rayon,nom,nom_image,prix) values(17,'Stylo Bic','bic',0.30);
insert into produit(idt_rayon,nom,nom_image,prix) values(17,'Cahier à carreaux','cahier',1.08);

insert into produit(idt_rayon,nom,nom_image,prix) values(18,'Poulet roti','poulet',3.45);
insert into produit(idt_rayon,nom,nom_image,prix) values(18,'Roti de Porc','roti',2.54);

alter table magasin owner to kore;
alter table rayon owner to kore;
alter table produit owner to kore;
alter table panier owner to kore;
alter table panier_item owner to kore;


insert into panier(idt_magasin,username) values(1,'patrice');
insert into panier_item(idt_panier,idt_produit,quantite) values(1,2,1);
insert into panier_item(idt_panier,idt_produit,quantite) values(1,5,5);
insert into panier_item(idt_panier,idt_produit,quantite) values(1,12,4);
insert into panier_item(idt_panier,idt_produit,quantite) values(1,15,2);




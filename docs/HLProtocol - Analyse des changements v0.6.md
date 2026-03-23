# Analyse des changements : HLProtocol original → CAPS v0.6_dev

## 1. Nouvelles transactions ajoutées (4)

| ID | Nom | Description |
|----|-----|-------------|
| **38** | Upload Folder | Nouvelle action d'upload de dossier (type `bundle`) |
| **39** | Download Folder *(Dowload — faute dans le doc)* | Nouvelle action de download de dossier (type `bundle`) |
| **40** | Can Send Message | Nouvelle transaction bundle liée aux dossiers |
| **348** | Get All Users | Requête client → server pour obtenir tous les comptes utilisateurs avec login, mot de passe, nom et accès |

> La section "Download Folder Actions" (description des actions send/resume/next pendant un transfert de dossier) a été **déplacée** depuis la transaction [37] vers ce bloc de nouvelles transactions.

---

## 2. Aucune transaction supprimée

Les 55 transactions existantes sont toutes conservées.

---

## 3. Changements de contenu substantiels

**[202] File Path** — description de la structure du champ complétée :
```
+ Directory level | 2 || Start at 0 (server root directory)
+ Parameter list contains multiple records:
+   ?? | 1 | 0 | Padding
+   Directory Name Size | 2 |
+   Directory Name | size |
```

**[212] File New Path** — ajout d'une note :
```
+ "This field is the same as field 202"
```

**[304] Set Client User Info** — les champs `113 Options` (Bitmap refuse chat/message) et `215 Automatic response` ont été **retirés** de la liste des champs du request (ne restent que `102 User name` et `104 User icon ID`).

**[354] User Access** — ajout d'un champ dans la reply :
```
+ 300 | User Name with info | optional
```

**[121] Agreed** — la description du comportement post-acknowledge ("After receiving server's acknowledgement, client sends Get User Name List...") a été **supprimée**.

---

## 4. Reformatages (sans changement de sémantique)

- **Tableaux** : passage du format aligné `| ID  | Field Name  | Note  |` → format compact `ID|Field Name|Note|`
- **Paragraphes** : les textes coupés sur plusieurs lignes dans l'original sont regroupés sur une seule ligne dans la version CAPS (artefact RTF vs Word)
- **`Constant:` / `Initiator:`** : indentation harmonisée (espaces → tabulations)

---

## Résumé

La version CAPS v0.6_dev ajoute principalement le **support transfert de dossiers** (upload/download folder avec leurs actions associées) et une nouvelle transaction **Get All Users** pour la gestion des comptes. Les autres modifications sont des clarifications mineures et un reformatage des tableaux.

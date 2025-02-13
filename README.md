# 🛴 Projet-QT - Trotinettes GPS

Un **système de réservation de trottinettes électriques** utilisant **QT pour l'interface**, **Arduino pour la récupération GPS**, et **C++ pour la gestion des réservations**.

---

## 🚀 Fonctionnalités

- 📌 **Suivi GPS en temps réel** : Arduino récupère la position GPS via un module **NEO-6M**.
- 🌍 **Vérification des zones autorisées** : L'application contrôle si la trottinette est dans la zone définie.
- 🛠️ **Interface graphique (QT)** : Affichage de la position des trottinettes et des réservations.
- 🔗 **Communication Arduino ↔ QT/C++** : Récupération des données GPS via **UART (Série)**.
- 📊 **Gestion des utilisateurs et réservations** : Création, modification et suppression des réservations.

---

## 🏗️ Technologies utilisées

- **QT** : Interface graphique pour la gestion des trottinettes et des réservations.
- **C++** : Gestion des données et communication avec l'Arduino.
- **Arduino** : Lecture des coordonnées GPS avec le module **NEO-6M**.
- **UART (Série)** : Transmission des coordonnées GPS à l’application QT.
- **Base de données (optionnel)** : SQLite ou MySQL pour stocker les trottinettes et utilisateurs.

---

## 📂 Structure du projet

Projet-QT/ │── src/ │ │── gps.hpp # Gestion des coordonnées GPS │ │── zone.hpp # Vérification des zones autorisées │ │── reservation.hpp # Gestion des réservations │ │── main.cpp # Lien entre QT et Arduino │── arduino/ │ │── gps_arduino.ino # Programme Arduino pour récupérer les données GPS │── ui/ │ │── mainwindow.ui # Interface graphique QT │ │── mainwindow.cpp # Logique de l'interface │── data/ │ │── trottinettes.db # Base de données SQLite (optionnel) │── README.md # Documentation du projet │── LICENSE # Licence d'utilisation

---

## 🔧 Matériel requis

- 🛠 **Carte Arduino** (Uno, Mega, etc.)
- 📡 **Module GPS NEO-6M** ou équivalent
- 💻 **QT Creator et Qt5**
- 🔌 **Câbles et alimentation**
- 📟 **Communication série (UART) activée**

---

## 📟 Installation et Exécution

### 🔹 1️⃣ Cloner le projet
```sh
git clone git@github.com:9adidaa/Projet-QT.git
cd Projet-QT

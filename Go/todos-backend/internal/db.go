package internal

import (
	"fmt"

	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
)

var (
	db *gorm.DB
)

func ConnectToDB(sqliteFileName string) {
	var err error
	db, err = gorm.Open(sqlite.Open(sqliteFileName), &gorm.Config{})
	if err != nil {
		panic(fmt.Sprintf("Failed to connect database, Error: %s", err.Error()))
	}
}

func AutoMigrate() {
	db.AutoMigrate(&Todo{})
}

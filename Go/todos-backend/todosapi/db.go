package todosapi

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
		panic(fmt.Sprint("Failed to connect database, Error: ", err))
	}
}

func AutoMigrate() {
	err := db.AutoMigrate(&Todo{})
	if err != nil {
		panic(fmt.Sprint("Error migrating database: ", err))
	}
}

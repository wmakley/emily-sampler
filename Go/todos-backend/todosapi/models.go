package todosapi

import (
	"time"

	"gorm.io/gorm"
)

type Todo struct {
	ID          uint           `json:"id" gorm:"primarykey"`
	Thing       string         `json:"thing" gorm:"type:text;not null"`
	CompletedAt *time.Time     `json:"completedAt"`
	CreatedAt   time.Time      `json:"createdAt"`
	UpdatedAt   time.Time      `json:"updatedAt"`
	DeletedAt   gorm.DeletedAt `json:"-" gorm:"index"`
}

type ErrorJson struct {
	Status  int    `json:"status"`
	Message string `json:"message"`
}

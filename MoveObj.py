def __draw(obj, ca, pic):
    ca.delete(obj.Id)
    obj.Id = ca.creat_image(obj.x, obj.y, image=pic)


class __MoveObj:
    def __init__(self, x, y, state, Id):
        self.x = x
        self.y = y
        self.state = state
        self.Id = Id
        self.HP = 100

    def ___move(self, dx, dy):
        self.x  = dx
        self.y  = dy

class pea(__MoveObj):
    pass

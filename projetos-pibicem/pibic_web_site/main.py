__author__ = 'klux'

import os
import jinja2
import webapp2
import smtplib
#import MySQLdb

template_dir = os.path.join(os.path.dirname(__file__), 'templates')
jinja_env = jinja2.Environment(loader = jinja2.FileSystemLoader(template_dir), autoescape = True)

class Handler(webapp2.RequestHandler):
    def write(self, *a, **kw):
        self.response.out.write(*a, **kw)

    def render_str(self, template, **params):
        t = jinja_env.get_template(template)
        return t.render(params)

    def render(self, template, **kw):
        self.write(self.render_str(template, **kw))

#class ConexaoBanco(object):
#
#     def __init__(self):
#        self.con = MySQLdb.connect('localhost', 'root', 'root', 'ar_condicionado')
#
#
#     def acess(self):
#        cursor = self.con.cursor()
#        return cursor

class Main(Handler):
    def get(self):
        self.render('telas/index.html')


class Sobre(Handler):
    def get(self):
        self.render("telas/sobre.html")

class Ar_1(Handler):

    def get(self):
#        c = ConexaoBanco()
#        cursor = c.acess()
#        cursor.execute('SELECT * FROM  real_time')
        #s = cursor.fetchone()[0]
        self.render('telas/graficos.html', sala=1)



class Ar_2(Handler):
    def get(self):
        self.render('telas/graficos.html', sala=2)

class Ar_3(Handler):
    def get(self):
        self.render('telas/graficos.html', sala=3)

class Ar_4(Handler):
    def get(self):
        self.render('telas/graficos.html', sala=4)

class Email(Handler):
    def get(self):
        pass
    def post(self):
    
        smtp = smtplib.SMTP_SSL('smtp.gmail.com', 465)
        smtp.login('railsonsantospb@gmail.com', 'thmpv77d')
        de = 'railsonsantospb@gmail.com'
        para = ['nosliarpb@hotmail.com']
        msg = """From: %s
        To: %s
        Subject: Buteco Open Source
        Email de teste do Buteco Open Source.""" % (de, ', '.join(para))
        smtp.sendmail(de, para, msg)
        smtp.quit()


app = webapp2.WSGIApplication([('/', Main),
                               ('/Sobre', Sobre),
                               ('/Ar_1', Ar_1),
                               ('/Ar_2', Ar_2),
                               ('/Ar_3', Ar_3),
                               ('/Ar_4', Ar_4)],
                            debug=True)